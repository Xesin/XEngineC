#include "stdafx.h"
#include "Renderer\Renderer.h"
#include "GameObjects\GameObject.h"
#include "Renderer\CachedImage.h"
#include "Managers\ScaleManager.h"
#include "Managers\CacheManager.h"


IWICImagingFactory* Renderer::wicFactory = nullptr;
ID2D1HwndRenderTarget* Renderer::m_pRenderTarget = nullptr;

Renderer::Renderer() :
	m_pDirect2dFactory(NULL),
	colorBrush(NULL),
	scaleManager(NULL)
{
	Renderer::wicFactory = NULL;
	Renderer::m_pRenderTarget = NULL;
}

Renderer::~Renderer()
{
	SafeRelease(&m_pDirect2dFactory);
	SafeRelease(&Renderer::m_pRenderTarget);
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

	if (!Renderer::m_pRenderTarget)
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
			&Renderer::m_pRenderTarget
		);

		scaleManager = new ScaleManager(size.width, size.height);


		if (SUCCEEDED(hr))
		{
			// Create a gray brush.
			hr = Renderer::m_pRenderTarget->CreateSolidColorBrush(
				D2D1::ColorF(D2D1::ColorF::LightSlateGray),
				&colorBrush
			);
		}

		CacheManager::GetInstance()->RefreshCache();
	}

	return hr;
}



HRESULT Renderer::OnRender(HWND m_hwnd, GameObject* gameObject)
{
	HRESULT hr = S_OK;

	hr = CreateDeviceResources(m_hwnd);
	if (SUCCEEDED(hr))
	{
		
		Renderer::m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

		gameObject->OnRender(this);

	}

	return hr;
}

void Renderer::PreRender()
{
	Renderer::m_pRenderTarget->BeginDraw();
	Renderer::m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));
}

void Renderer::EndRender()
{
	HRESULT hr = Renderer::m_pRenderTarget->EndDraw();

	if (hr == D2DERR_RECREATE_TARGET)
	{
		hr = S_OK;
		DiscardDeviceResources();
	}
}

void Renderer::RenderRect(float posX, float posY, float width, float height, D2D1::ColorF color, bool fill, float strokeWith)
{
	D2D1_RECT_F rectangle = D2D1::RectF(
		PixelsToDipsX(posX),
		PixelsToDipsY(posY),
		PixelsToDipsX(posX) + PixelsToDipsX(width),
		PixelsToDipsY(posY) + PixelsToDipsY(height)
	);

	colorBrush->SetColor(color);

	if (fill) {
		Renderer::m_pRenderTarget->FillRectangle(&rectangle, colorBrush);
	}
	else {
		Renderer::m_pRenderTarget->DrawRectangle(&rectangle, colorBrush, PixelsToDipsX(strokeWith));
	}
}

void Renderer::RenderCircle(float posX, float posY, float radiusX, float radiusY, D2D1::ColorF color, bool fill, float strokeWith)
{
	D2D1_ELLIPSE ellipse = D2D1::Ellipse(D2D1::Point2(PixelsToDipsX(posX), PixelsToDipsY(posY)),
		PixelsToDipsX(radiusX),
		PixelsToDipsY(radiusY));

	colorBrush->SetColor(color);
	if (fill) {
		Renderer::m_pRenderTarget->FillEllipse(&ellipse, colorBrush);
	}
	else {
		Renderer::m_pRenderTarget->DrawEllipse(&ellipse, colorBrush, PixelsToDipsX(strokeWith));
	}
}

void Renderer::RenderImage(float posX, float posY, CachedImage* imageToRender)
{
	
	ID2D1Bitmap* bitmapToRender = imageToRender->Get2D2Bitmap();
	if (bitmapToRender)
	{
		D2D1_RECT_F rectangle = D2D1::RectF(
			PixelsToDipsX(posX),
			PixelsToDipsY(posY),
			PixelsToDipsX(posX) + PixelsToDipsX(bitmapToRender->GetPixelSize().width),
			PixelsToDipsY(posY) + PixelsToDipsY(bitmapToRender->GetPixelSize().height)
		);
	
		Renderer::m_pRenderTarget->DrawBitmap(bitmapToRender, rectangle);
	}
}



void Renderer::OnResize(UINT width, UINT height)
{
	if (Renderer::m_pRenderTarget)
	{
		// Note: This method can fail, but it's okay to ignore the
		// error here, because the error will be returned again
		// the next time EndDraw is called.
		Renderer::m_pRenderTarget->Resize(D2D1::SizeU(width, height));
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

ID2D1HwndRenderTarget * Renderer::GetRenderTarget()
{
	return Renderer::m_pRenderTarget;
}

void Renderer::DiscardDeviceResources()
{
	SafeRelease(&Renderer::m_pRenderTarget);
	SafeRelease(&colorBrush);
}
