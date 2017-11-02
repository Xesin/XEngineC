#include "stdafx.h"
#include "Renderer\Renderer.h"
#include "GameObjects\GameObject.h"
#include "Renderer\CachedImage.h"
#include "Renderer\Camera.h"
#include "Managers\ScaleManager.h"
#include "Managers\CacheManager.h"

IWICImagingFactory* Renderer::wicFactory = nullptr;
ID2D1HwndRenderTarget* Renderer::renderTarget = nullptr;

Renderer::Renderer() :
	m_pDirect2dFactory(NULL),
	colorBrush(NULL),
	scaleManager(NULL),
	camera(NULL),
	tilledBrush(NULL)
{
	Renderer::wicFactory = NULL;
	Renderer::renderTarget = NULL;
}

Renderer::~Renderer()
{
	SafeRelease(&m_pDirect2dFactory);
	SafeRelease(&Renderer::renderTarget);
	SafeRelease(&colorBrush);
	SafeRelease(&Renderer::wicFactory);
	delete scaleManager;
}

HRESULT Renderer::Initialize(Camera* newCamera)
{
	FLOAT dpiX;
	FLOAT dpiY;
	HRESULT hr = CreateDeviceIndependentResources();
	if (SUCCEEDED(hr)) {
		GetDesktopDpi(&dpiX, &dpiY);
		DPIScaleX = dpiX / 96.0f;
		DPIScaleY = dpiY / 96.0f;

		camera = newCamera;
	}

	return hr;
}

void Renderer::GetDesktopDpi(FLOAT *dpiX, FLOAT* dpiY) {
	m_pDirect2dFactory->GetDesktopDpi(dpiX, dpiY);
}

HRESULT Renderer::CreateDeviceResources(HWND m_hwnd)
{
	HRESULT hr = S_OK;
	if (!Renderer::renderTarget)
	{
		RECT rc;
		GetClientRect(m_hwnd, &rc);

		D2D1_SIZE_U size = D2D1::SizeU(
			rc.right - rc.left,
			rc.bottom - rc.top
		);

		// Create a Direct2D render target.
		D2D1_RENDER_TARGET_PROPERTIES properties = D2D1::RenderTargetProperties();
		properties.pixelFormat = D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED);
		hr = m_pDirect2dFactory->CreateHwndRenderTarget(
			properties,
			D2D1::HwndRenderTargetProperties(m_hwnd, size),
			&Renderer::renderTarget
		);

		scaleManager = new ScaleManager(size.width, size.height);


		if (SUCCEEDED(hr))
		{
			// Create a gray brush.
			hr = Renderer::renderTarget->CreateSolidColorBrush(
				D2D1::ColorF(D2D1::ColorF::LightSlateGray),
				&colorBrush
			);
			
		}
		
		//LEFT BOTTOM COORDINATES MATRIX
		UINT32 canvasHeight = renderTarget->GetPixelSize().height;
		canvasScaleMatrix = D2D1::Matrix3x2F::Scale(1.0, -1.0f);
		canvasTranslationMatrix = D2D1::Matrix3x2F::Translation(
			0.f,
			(float) canvasHeight
		);

		CacheManager::GetInstance()->RefreshCache();
	}

	return hr;
}

void Renderer::SetClearColor(D2D1::ColorF color)
{
	clearColor = color;
}

HRESULT Renderer::CreateDeviceIndependentResources()
{
	HRESULT hr = S_OK;

	// Create a Direct2D factory.
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pDirect2dFactory);

	if (SUCCEEDED(hr)) {
		// Initialize COM
		hr = CreateIwicFactory();
	}

	return hr;
}

IWICImagingFactory * Renderer::GetIwicFactory()
{
	if (Renderer::wicFactory == nullptr) {
		CreateIwicFactory();
	}
	return Renderer::wicFactory;
}

HRESULT Renderer::CreateIwicFactory()
{
	CoInitialize(NULL);

	// Create the COM imaging factory
	HRESULT hr = CoCreateInstance(
		CLSID_WICImagingFactory,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&Renderer::wicFactory)
	);

	return hr;
}

ID2D1HwndRenderTarget * Renderer::GetRenderTarget()
{
	return Renderer::renderTarget;
}

void Renderer::OnRenderGroup(ArrayList<GameObject*> &gameObjects)
{
	Renderer &ref = *this;
	
	for (int i = 0; i < gameObjects.size; i++) {
		GameObject& go = *gameObjects[i];
		if (!go.isPendingDestroy && &go != nullptr) {
			go.OnRender(ref);
		}
		else {
			gameObjects.erase(i);
		}
	}
}

HRESULT Renderer::PreRender(HWND m_hwnd)
{
	HRESULT hr = S_OK;
	hr = CreateDeviceResources(m_hwnd);
	if (SUCCEEDED(hr)) {
		Renderer::renderTarget->BeginDraw();
		Renderer::renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
		Renderer::renderTarget->Clear(clearColor);
	}
	return hr;
}

void Renderer::EndRender()
{
	HRESULT hr = Renderer::renderTarget->EndDraw();

	if (hr == D2DERR_RECREATE_TARGET)
	{
		hr = S_OK;
		DiscardDeviceResources();
	}
}

void Renderer::SetTransform(D2D1::Matrix3x2F transform)
{
	//APPLY LEFT BOTTOM COORDINATES
	renderTarget->SetTransform(transform * canvasScaleMatrix * canvasTranslationMatrix);
}

void Renderer::SetTransform(Vector2 position, Vector2 bounds, Vector2 scale, Vector2 anchor, float angle)
{
	Vector2 invertedScale = Vector2(scale.x, scale.y * -1.f);
	D2D1_SIZE_F correctedScale = D2D1::SizeF(invertedScale.x * scaleManager->gameScale.x, invertedScale.y * scaleManager->gameScale.y);
	D2D1::Matrix3x2F scaleMatrix = D2D1::Matrix3x2F::Scale(correctedScale);
	float32 x = position.x;
	float32 y = position.y;
	//LEFT BOTTOM COORDINATES
	D2D1::Matrix3x2F rotationMatrix = D2D1::Matrix3x2F::Rotation(
		angle,
		D2D1::Point2F(
		(x - abs((camera->position.x - camera->bounds.x / 2.f))) * scaleManager->renderTargetScaleX * scaleManager->gameScale.x,
			(y - abs((camera->position.y - camera->bounds.y / 2.f))) * scaleManager->renderTargetScaleY * scaleManager->gameScale.y
		)
	);

	Vector2 pos = GetCameraPos();

	D2D1::Matrix3x2F translationMatrix = D2D1::Matrix3x2F::Translation(
		((-anchor.x * scaleManager->gameScale.x) * invertedScale.x - pos.x)* (scaleManager->renderTargetScaleX),
		((-anchor.y * scaleManager->gameScale.y) * invertedScale.y - pos.y) * (scaleManager->renderTargetScaleY)
	);
	scaleMatrix = scaleMatrix * translationMatrix * rotationMatrix;
	SetTransform(scaleMatrix);
}

void Renderer::RenderRect(float posX, float posY, int width, int height, D2D1::ColorF color, Vector2 scale, bool fill, float strokeWith)
{
	scale.y *= -1.f;
	D2D1_RECT_F rectangle = D2D1::RectF(
		PixelsToDipsX(posX / scale.x),
		PixelsToDipsY(posY / scale.y),
		PixelsToDipsX(posX / scale.x + width),
		PixelsToDipsY(posY / scale.y + height)
	);

	colorBrush->SetColor(color);

	if (fill) {
		Renderer::renderTarget->FillRectangle(&rectangle, colorBrush);
	}
	else {
		Renderer::renderTarget->DrawRectangle(&rectangle, colorBrush, PixelsToDipsX(strokeWith));
	}
}

void Renderer::RenderCircle(float posX, float posY, float radiusX, float radiusY, D2D1::ColorF color, Vector2 scale, bool fill, float strokeWith)
{
	scale.y *= -1.f;
	D2D1_ELLIPSE ellipse = D2D1::Ellipse(D2D1::Point2(
		PixelsToDipsX(posX / scale.x),
		PixelsToDipsY(posY / scale.y)),
		PixelsToDipsX(radiusX),
		PixelsToDipsY(radiusY));

	colorBrush->SetColor(color);
	if (fill) {
		Renderer::renderTarget->FillEllipse(&ellipse, colorBrush);
	}
	else {
		Renderer::renderTarget->DrawEllipse(&ellipse, colorBrush, PixelsToDipsX(strokeWith));
	}
}

void Renderer::RenderImage(float posX, float posY, CachedImage &imageToRender, int frameColumn, int frameRow, int frame, int frameWidth, int frameHeight, Vector2 scale)
{
	scale.y *= -1.f;
	ID2D1Bitmap* bitmapToRender = imageToRender.Get2D2Bitmap();
	if (bitmapToRender)
	{
		D2D1_RECT_F src = D2D1::RectF(
			(float) frameColumn * frameWidth,
			(float) frameRow * frameHeight,
			(float) frameColumn * frameWidth + frameWidth,
			(float) frameRow * frameHeight + frameHeight
		);

		D2D1_RECT_F destination = D2D1::RectF(
			PixelsToDipsX(posX / scale.x) ,
			PixelsToDipsY(posY / scale.y),
			PixelsToDipsX(posX / scale.x + frameWidth),
			PixelsToDipsY(posY / scale.y + frameHeight)
		);
	
		renderTarget->DrawBitmap(
			bitmapToRender, 
			destination, 
			1.0f, 
			D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, 
			src
		);
	}
}

void Renderer::RenderTilledImage(Vector2 position, CachedImage &imageToRender, int frameColumn, int frameRow, int frame, Vector2 frameSize, Vector2 tileScroll, Vector2 tileSize, Vector2 anchor)
{
	ID2D1BitmapBrush *m_pBitmapBrush;

	CreateTilledBitmapBrush(imageToRender, frameColumn, frameRow, frame, frameSize, &m_pBitmapBrush, tileScroll);

	Vector2 pos = GetCameraPos();

	D2D1::Matrix3x2F translationMatrix = D2D1::Matrix3x2F::Translation(
		-pos.x,
		-pos.y
	);
	
	SetTransform(translationMatrix);
	renderTarget->FillRectangle(
		D2D1::RectF(
			position.x - anchor.x, 
			position.y - anchor.y, 
			position.x - anchor.x + tileSize.x, 
			position.y - anchor.y + tileSize.y
		),
		m_pBitmapBrush
	);

	SafeRelease(&m_pBitmapBrush);
}

void Renderer::CreateTilledBitmapBrush(CachedImage & imageToRender, int frameColumn, int frameRow, int frame, Vector2 frameSize, ID2D1BitmapBrush **m_pBitmapBrush, Vector2 tileScroll)
{
	ID2D1Bitmap* bitmapToRender = imageToRender.Get2D2Bitmap();
	if (bitmapToRender)
	{
		D2D1_RECT_F src = D2D1::RectF(
			(float)frameColumn * frameSize.x ,
			(float)frameRow * frameSize.y ,
			(float)frameColumn * frameSize.x + frameSize.x ,
			(float)frameRow * frameSize.y + frameSize.y
		);

		D2D1_RECT_F destination = D2D1::RectF(
			PixelsToDipsX(0.f),
			PixelsToDipsY(0.f),
			PixelsToDipsX(frameSize.x),
			PixelsToDipsY(frameSize.y)
		);

		ID2D1BitmapRenderTarget *pCompatibleRenderTarget = NULL;
		HRESULT hr = renderTarget->CreateCompatibleRenderTarget(
			D2D1::SizeF(frameSize.x, frameSize.y),
			&pCompatibleRenderTarget
		);
		if (SUCCEEDED(hr))
		{
			pCompatibleRenderTarget->BeginDraw();
			pCompatibleRenderTarget->Clear();
			//APPLY LEFT BOTTOM COORDINATES

			D2D1::Matrix3x2F translation = D2D1::Matrix3x2F::Translation(
				0.f,
				(float)frameSize.y
			);

			pCompatibleRenderTarget->SetTransform(canvasScaleMatrix * translation);
			pCompatibleRenderTarget->DrawBitmap(
				bitmapToRender,
				destination,
				1.0f,
				D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR,
				src
			);
			pCompatibleRenderTarget->EndDraw();

			// Retrieve the bitmap from the render target.
			ID2D1Bitmap *pGridBitmap = NULL;
			hr = pCompatibleRenderTarget->GetBitmap(&pGridBitmap);
			if (SUCCEEDED(hr))
			{
				// Choose the tiling mode for the bitmap brush.
				D2D1_BITMAP_BRUSH_PROPERTIES brushProperties =
					D2D1::BitmapBrushProperties(D2D1_EXTEND_MODE_WRAP, D2D1_EXTEND_MODE_WRAP);

				hr = renderTarget->CreateBitmapBrush(pGridBitmap, brushProperties, m_pBitmapBrush);

				pGridBitmap->Release();
			}
			pCompatibleRenderTarget->Release();
		}
	}
}

void Renderer::OnResize(UINT width, UINT height)
{
	if (Renderer::renderTarget)
	{
		// Note: This method can fail, but it's okay to ignore the
		// error here, because the error will be returned again
		// the next time EndDraw is called.
		Renderer::renderTarget->Resize(D2D1::SizeU(width, height));
		scaleManager->OnResize(width);
	}
}

void Renderer::DrawPolygon(const b2Vec2 * vertices, int32 vertexCount, const b2Color & color)
{
	int i;
	ID2D1PathGeometry *geo;
	ID2D1GeometrySink *sink;
	ID2D1SolidColorBrush *brush;
	D2D1::ColorF dColor(color.r, color.g, color.b, 0.7f);
	D2D1_POINT_2F *points = new D2D1_POINT_2F[vertexCount + 1];
	HRESULT hr;

	Vector2 pos = GetCameraPos();

	D2D1::Matrix3x2F translationMatrix = D2D1::Matrix3x2F::Translation(
		-pos.x,
		-pos.y
	);

	D2D1::Matrix3x2F scaleMatrix = D2D1::Matrix3x2F::Scale(D2D1::SizeF(scaleManager->renderTargetScaleX, scaleManager->renderTargetScaleY));
	SetTransform(translationMatrix * scaleMatrix);
	// create a direct2d pathGeometry
	hr = m_pDirect2dFactory->CreatePathGeometry(&geo);
	hr = geo->Open(&sink);
	sink->SetFillMode(D2D1_FILL_MODE_WINDING);
	// first point
	sink->BeginFigure(D2D1::Point2F(WorldToScreenPixels(vertices[0].x * scaleManager->gameScale.x), WorldToScreenPixels(vertices[0].y * scaleManager->gameScale.y)), D2D1_FIGURE_BEGIN_FILLED);
	// middle points
	vertices++;
	vertexCount--;
	for (i = 0; i < vertexCount; i++, vertices++)
	{
		points[i].x = WorldToScreenPixels(vertices->x * scaleManager->gameScale.x);
		points[i].y = WorldToScreenPixels(vertices->y * scaleManager->gameScale.y);
	}
	points[vertexCount].x = WorldToScreenPixels(points[0].x * scaleManager->gameScale.x);
	points[vertexCount].y = WorldToScreenPixels(points[0].y * scaleManager->gameScale.y);
	sink->AddLines(points, vertexCount);
	// close it
	sink->EndFigure(D2D1_FIGURE_END_CLOSED);
	sink->Close();
	SafeRelease(&sink);

	renderTarget->CreateSolidColorBrush(dColor, &brush);
	renderTarget->DrawGeometry(geo, brush);

	delete[] points;
	SafeRelease(&geo);
	SafeRelease(&brush);
}

void Renderer::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	int i;
	ID2D1PathGeometry *geo;
	ID2D1GeometrySink *sink;
	ID2D1SolidColorBrush *brush;
	D2D1::ColorF dColor(color.r, color.g, color.b, 0.7f);
	D2D1_POINT_2F *points = new D2D1_POINT_2F[vertexCount + 1];
	HRESULT hr;

	Vector2 pos = GetCameraPos();

	D2D1::Matrix3x2F translationMatrix = D2D1::Matrix3x2F::Translation(
		-pos.x,
		-pos.y
	);
	D2D1::Matrix3x2F scaleMatrix = D2D1::Matrix3x2F::Scale(D2D1::SizeF(scaleManager->renderTargetScaleX, scaleManager->renderTargetScaleY));
	SetTransform(translationMatrix * scaleMatrix);
	// create a direct2d pathGeometry
	hr = m_pDirect2dFactory->CreatePathGeometry(&geo);
	hr = geo->Open(&sink);
	sink->SetFillMode(D2D1_FILL_MODE_WINDING);
	// first point
	sink->BeginFigure(D2D1::Point2F(WorldToScreenPixels(vertices[0].x * scaleManager->gameScale.x), WorldToScreenPixels(vertices[0].y * scaleManager->gameScale.y)), D2D1_FIGURE_BEGIN_FILLED);
	// middle points
	vertices++;
	vertexCount--;
	for (i = 0; i < vertexCount; i++, vertices++)
	{
		points[i].x = WorldToScreenPixels(vertices->x * scaleManager->gameScale.x);
		points[i].y = WorldToScreenPixels(vertices->y * scaleManager->gameScale.y);
	}
	points[vertexCount].x = WorldToScreenPixels(points[0].x * scaleManager->gameScale.x);
	points[vertexCount].y = WorldToScreenPixels(points[0].y * scaleManager->gameScale.y);
	sink->AddLines(points, vertexCount);
	// close it
	sink->EndFigure(D2D1_FIGURE_END_CLOSED);
	sink->Close();
	SafeRelease(&sink);

	renderTarget->CreateSolidColorBrush(dColor, &brush);
	renderTarget->FillGeometry(geo, brush);

	delete[] points;
	SafeRelease(&geo);
	SafeRelease(&brush);
}

void Renderer::DrawCircle(const b2Vec2 & center, float32 radius, const b2Color & color)
{
	Vector2 pos = GetCameraPos();

	D2D1::Matrix3x2F translationMatrix = D2D1::Matrix3x2F::Translation(
		-pos.x,
		-pos.y
	);
	SetTransform(translationMatrix);
	b2Vec2 centerScreen = WorldToScreenPixels(center);
	D2D1::ColorF dColor(color.r, color.g, color.b, 0.7f);
	float32 radiusScreen = WorldToScreenPixels(radius);
	Vector2 scale;
	scale.y = -1.f;
	scale.x = 1.f;
	RenderCircle(centerScreen.x * scaleManager->gameScale.x, centerScreen.y * scaleManager->gameScale.y, radiusScreen * scaleManager->gameScale.x, radiusScreen * scaleManager->gameScale.y, dColor, scale, false);
}

void Renderer::DrawSolidCircle(const b2Vec2 & center, float32 radius, const b2Vec2 & axis, const b2Color & color)
{
	Vector2 pos = GetCameraPos();

	D2D1::Matrix3x2F translationMatrix = D2D1::Matrix3x2F::Translation(
		-pos.x,
		-pos.y
	);
	SetTransform(translationMatrix);
	b2Vec2 centerScreen = WorldToScreenPixels(center);
	D2D1::ColorF dColor(color.r, color.g, color.b, 0.7f);
	float32 radiusScreen = WorldToScreenPixels(radius);
	Vector2 scale;
	scale.y = -1.f;
	scale.x = 1.f;
	RenderCircle(centerScreen.x * scaleManager->gameScale.x, centerScreen.y * scaleManager->gameScale.y, radiusScreen * scaleManager->gameScale.x, radiusScreen * scaleManager->gameScale.y, dColor, scale, true);
}

void Renderer::DrawSegment(const b2Vec2 & p1, const b2Vec2 & p2, const b2Color & color)
{
	Vector2 pos = GetCameraPos();

	D2D1::Matrix3x2F translationMatrix = D2D1::Matrix3x2F::Translation(
		-pos.x,
		-pos.y
	);
	D2D1::Matrix3x2F scaleMatrix = D2D1::Matrix3x2F::Scale(D2D1::SizeF(scaleManager->renderTargetScaleX, scaleManager->renderTargetScaleY));
	SetTransform(translationMatrix * scaleMatrix);
	b2Vec2 p1Screen = WorldToScreenPixels(p1);
	b2Vec2 p2Screen = WorldToScreenPixels(p2);
	D2D1::ColorF dColor(color.r, color.g, color.b, 0.7f);
	colorBrush->SetColor(dColor);
	renderTarget->DrawLine(D2D1::Point2F(p1Screen.x * scaleManager->gameScale.x, p1Screen.y * scaleManager->gameScale.y), D2D1::Point2F(p2Screen.x * scaleManager->gameScale.x, p2Screen.y * scaleManager->gameScale.y), colorBrush);
}

void Renderer::DrawTransform(const b2Transform & xf)
{
	const float32 k_axisScale = 0.4f;
	b2Color red(1.0f, 0.0f, 0.0f);
	b2Color green(0.0f, 1.0f, 0.0f);
	b2Vec2 p1 = xf.p, p2;
	b2Vec2 p1Screen, p2Screen;

	Vector2 pos = GetCameraPos();

	D2D1::Matrix3x2F translationMatrix = D2D1::Matrix3x2F::Translation(
		-pos.x,
		-pos.y
	);
	D2D1::Matrix3x2F scaleMatrix = D2D1::Matrix3x2F::Scale(D2D1::SizeF(scaleManager->renderTargetScaleX, scaleManager->renderTargetScaleY));
	SetTransform(translationMatrix * scaleMatrix);

	D2D1::ColorF dColorRed(red.r, red.g, red.b, 0.7f);
	colorBrush->SetColor(dColorRed);
	p2 = p1 + k_axisScale * xf.q.GetXAxis();
	p1Screen = WorldToScreenPixels(p1);
	p2Screen = WorldToScreenPixels(p2);
	renderTarget->DrawLine(D2D1::Point2F(p1Screen.x * scaleManager->gameScale.x, p1Screen.y * scaleManager->gameScale.y), D2D1::Point2F(p2Screen.x * scaleManager->gameScale.x, p2Screen.y * scaleManager->gameScale.y), colorBrush);

	D2D1::ColorF dColorGreen(green.r, green.g, green.b, 0.7f);
	colorBrush->SetColor(dColorGreen);
	p2 = p1 + k_axisScale * xf.q.GetYAxis();
	p1Screen = WorldToScreenPixels(p1);
	p2Screen = WorldToScreenPixels(p2);
	renderTarget->DrawLine(D2D1::Point2F(p1Screen.x * scaleManager->gameScale.x, p1Screen.y * scaleManager->gameScale.y), D2D1::Point2F(p2Screen.x * scaleManager->gameScale.x, p2Screen.y * scaleManager->gameScale.y), colorBrush);

	
}

void Renderer::DrawPoint(const b2Vec2 & p, float32 size, const b2Color & color)
{
}

void Renderer::DiscardDeviceResources()
{
	SafeRelease(&Renderer::renderTarget);
	SafeRelease(&colorBrush);
}

Vector2 Renderer::GetCameraPos()
{
	Vector2 pos = Vector2();

	pos.x = (camera->position.x  * scaleManager->gameScale.x);
	pos.y = (camera->position.y * scaleManager->gameScale.y);

	return pos;
}

b2Vec2 Renderer::WorldToScreenPixels(b2Vec2 worldUnit)
{
	b2Vec2 pu = (b2Vec2(floor(50.f * worldUnit.x), floor(50.f * worldUnit.y)));
	return pu;
}

b2Vec2 Renderer::PixelsToWorldUnits(b2Vec2 screenPixel)
{
	b2Vec2 pu = b2Vec2(0.02f * screenPixel.x, 0.02f * screenPixel.y);
	return pu;
}

Vector2 Renderer::WorldToScreenPixels(Vector2 worldUnit)
{
	Vector2 pu = (Vector2(floor(50.f * worldUnit.x), floor(50.f * worldUnit.y)));
	return pu;
}

Vector2 Renderer::PixelsToWorldUnits(Vector2 screenPixel)
{
	Vector2 pu = Vector2(0.02f * screenPixel.x, 0.02f * screenPixel.y);
	return pu;
}

float Renderer::WorldToScreenPixels(float worldUnit)
{
	float32 pu = floor(50.f * worldUnit);
	return pu;
}

float Renderer::PixelsToWorldUnits(float screenPixel)
{
	float32 pu = 0.02f * screenPixel;
	return pu;
}