#include "stdafx.h"
#include "Renderer\Renderer.h"
#include "GameObjects\GameObject.h"
#include "Renderer\CachedImage.h"
#include "Managers\ScaleManager.h"
#include "Managers\CacheManager.h"

IWICImagingFactory* Renderer::wicFactory = nullptr;
ID2D1HwndRenderTarget* Renderer::renderTarget = nullptr;

Renderer::Renderer() :
	m_pDirect2dFactory(NULL),
	colorBrush(NULL),
	scaleManager(NULL)
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

HRESULT Renderer::Initialize()
{
	return CreateDeviceIndependentResources();
	FLOAT dpiX;
	FLOAT dpiY;

	GetDesktopDpi(&dpiX, &dpiY);

	DPIScaleX = dpiX / 96.0f;
	DPIScaleY = dpiY / 96.0f;
}

void Renderer::GetDesktopDpi(FLOAT *dpiX, FLOAT* dpiY) {
	m_pDirect2dFactory->GetDesktopDpi(dpiX, dpiY);
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



void Renderer::OnRenderGroup(ArrayList<GameObject*> &gameObjects)
{
	Renderer &ref = *this;
	
	for (int i = 0; i < gameObjects.size; i++) {
		GameObject& go = *gameObjects[i];
		if (!go.isPendingDestroy) {
			go.OnRender(ref);
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
		Renderer::renderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));
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

void Renderer::RenderRect(float posX, float posY, int width, int height, D2D1::ColorF color, D2D_SIZE_F scale, bool fill, float strokeWith)
{
	D2D1_RECT_F rectangle = D2D1::RectF(
		PixelsToDipsX(posX / scale.width),
		PixelsToDipsY(posY / scale.height),
		PixelsToDipsX(posX / scale.width + width),
		PixelsToDipsY(posY / scale.height + height)
	);

	colorBrush->SetColor(color);

	if (fill) {
		Renderer::renderTarget->FillRectangle(&rectangle, colorBrush);
	}
	else {
		Renderer::renderTarget->DrawRectangle(&rectangle, colorBrush, PixelsToDipsX(strokeWith));
	}
}

void Renderer::RenderCircle(float posX, float posY, float radiusX, float radiusY, D2D1::ColorF color, D2D_SIZE_F scale, bool fill, float strokeWith)
{
	D2D1_ELLIPSE ellipse = D2D1::Ellipse(D2D1::Point2(
		PixelsToDipsX(posX / scale.width),
		PixelsToDipsY(posY / scale.height)),
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

void Renderer::RenderImage(float posX, float posY, CachedImage &imageToRender, int frameColumn, int frameRow, int frame, int frameWidth, int frameHeight, D2D_SIZE_F scale)
{
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
			PixelsToDipsX(posX / scale.width) ,
			PixelsToDipsY(posY / scale.height),
			PixelsToDipsX(posX / scale.width + frameWidth),
			PixelsToDipsY(posY / scale.height + frameHeight)
		);
	
		Renderer::renderTarget->DrawBitmap(
			bitmapToRender, 
			destination, 
			1.0f, 
			D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, 
			src
		);
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

void Renderer::DrawPolygon(const b2Vec2 * vertices, int32 vertexCount, const b2Color & color)
{
	int i;
	ID2D1PathGeometry *geo;
	ID2D1GeometrySink *sink;
	ID2D1SolidColorBrush *brush;
	D2D1::ColorF dColor(color.r, color.g, color.b, 0.7f);
	D2D1_POINT_2F *points = new D2D1_POINT_2F[vertexCount + 1];
	HRESULT hr;
	SetTransform(D2D1::Matrix3x2F::Identity());
	// create a direct2d pathGeometry
	hr = m_pDirect2dFactory->CreatePathGeometry(&geo);
	hr = geo->Open(&sink);
	sink->SetFillMode(D2D1_FILL_MODE_WINDING);
	// first point
	sink->BeginFigure(D2D1::Point2F(WorldToScreenPixels(vertices[0].x), WorldToScreenPixels(vertices[0].y)), D2D1_FIGURE_BEGIN_FILLED);
	// middle points
	vertices++;
	vertexCount--;
	for (i = 0; i < vertexCount; i++, vertices++)
	{
		points[i].x = WorldToScreenPixels(vertices->x);
		points[i].y = WorldToScreenPixels(vertices->y);
	}
	points[vertexCount].x = WorldToScreenPixels(points[0].x);
	points[vertexCount].y = WorldToScreenPixels(points[0].y);
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
	SetTransform(D2D1::Matrix3x2F::Identity());
	// create a direct2d pathGeometry
	hr = m_pDirect2dFactory->CreatePathGeometry(&geo);
	hr = geo->Open(&sink);
	sink->SetFillMode(D2D1_FILL_MODE_WINDING);
	// first point
	sink->BeginFigure(D2D1::Point2F(WorldToScreenPixels(vertices[0].x), WorldToScreenPixels(vertices[0].y)), D2D1_FIGURE_BEGIN_FILLED);
	// middle points
	vertices++;
	vertexCount--;
	for (i = 0; i < vertexCount; i++, vertices++)
	{
		points[i].x = WorldToScreenPixels(vertices->x);
		points[i].y = WorldToScreenPixels(vertices->y);
	}
	points[vertexCount].x = WorldToScreenPixels(points[0].x);
	points[vertexCount].y = WorldToScreenPixels(points[0].y);
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
	b2Vec2 centerScreen = WorldToScreenPixels(center);
	D2D1::ColorF dColor(color.r, color.g, color.b, 0.7f);
	float32 radiusScreen = WorldToScreenPixels(radius);
	D2D1_SIZE_F scale;
	scale.height = 1.f;
	scale.width = 1.f;
	RenderCircle(centerScreen.x, centerScreen.y, radiusScreen, radiusScreen, dColor, scale, false);
}

void Renderer::DrawSolidCircle(const b2Vec2 & center, float32 radius, const b2Vec2 & axis, const b2Color & color)
{
	b2Vec2 centerScreen = WorldToScreenPixels(center);
	D2D1::ColorF dColor(color.r, color.g, color.b, 0.7f);
	float32 radiusScreen = WorldToScreenPixels(radius);
	D2D1_SIZE_F scale;
	scale.height = 1.f;
	scale.width = 1.f;
	RenderCircle(centerScreen.x, centerScreen.y, radiusScreen, radiusScreen, dColor, scale, true);
}

void Renderer::DrawSegment(const b2Vec2 & p1, const b2Vec2 & p2, const b2Color & color)
{
	b2Vec2 p1Screen = WorldToScreenPixels(p1);
	b2Vec2 p2Screen = WorldToScreenPixels(p2);
	D2D1::ColorF dColor(color.r, color.g, color.b, 0.7f);
	colorBrush->SetColor(dColor);
	renderTarget->DrawLine(D2D1::Point2F(p1Screen.x, p1Screen.y), D2D1::Point2F(p2Screen.x, p2Screen.y), colorBrush);
}

void Renderer::DrawTransform(const b2Transform & xf)
{
	const float32 k_axisScale = 0.4f;
	b2Color red(1.0f, 0.0f, 0.0f);
	b2Color green(0.0f, 1.0f, 0.0f);
	b2Vec2 p1 = xf.p, p2;
	b2Vec2 p1Screen, p2Screen;

	D2D1::ColorF dColorRed(red.r, red.g, red.b, 0.7f);
	colorBrush->SetColor(dColorRed);
	p2 = p1 + k_axisScale * xf.q.GetXAxis();
	p1Screen = WorldToScreenPixels(p1);
	p2Screen = WorldToScreenPixels(p2);
	renderTarget->DrawLine(D2D1::Point2F(p1Screen.x, p1Screen.y), D2D1::Point2F(p2Screen.x, p2Screen.y), colorBrush);

	D2D1::ColorF dColorGreen(green.r, green.g, green.b, 0.7f);
	colorBrush->SetColor(dColorGreen);
	p2 = p1 + k_axisScale * xf.q.GetYAxis();
	p1Screen = WorldToScreenPixels(p1);
	p2Screen = WorldToScreenPixels(p2);
	renderTarget->DrawLine(D2D1::Point2F(p1Screen.x, p1Screen.y), D2D1::Point2F(p2Screen.x, p2Screen.y), colorBrush);

	
}

void Renderer::DrawPoint(const b2Vec2 & p, float32 size, const b2Color & color)
{
}

ID2D1HwndRenderTarget * Renderer::GetRenderTarget()
{
	return Renderer::renderTarget;
}

void Renderer::DiscardDeviceResources()
{
	SafeRelease(&Renderer::renderTarget);
	SafeRelease(&colorBrush);
}


b2Vec2 Renderer::WorldToScreenPixels(b2Vec2 worldUnit)
{
	b2Vec2 pu = (b2Vec2(floor(50.f * worldUnit.x), floor(50.f * worldUnit.y)));
	return pu;
}

b2Vec2 Renderer::ScreenToWorldUnits(b2Vec2 screenPixel)
{
	b2Vec2 pu = b2Vec2(0.02f * screenPixel.x, 0.02f * screenPixel.y);
	return pu;
}

float Renderer::WorldToScreenPixels(float worldUnit)
{
	float32 pu = floor(50.f * worldUnit);
	return pu;
}

float Renderer::ScreenToWorldUnits(float screenPixel)
{
	float32 pu = 0.02f * screenPixel;
	return pu;
}