#include "stdafx.h"
#include "Renderer.h"
#include "GameObject.h"
#include <wincodec.h>

Renderer::Renderer() :
	m_pDirect2dFactory(NULL),
	m_pRenderTarget(NULL),
	colorBrush(NULL)
{
}

Renderer::~Renderer()
{
	SafeRelease(&m_pDirect2dFactory);
	SafeRelease(&m_pRenderTarget);
	SafeRelease(&colorBrush);
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

	return hr;
}

HRESULT Renderer::CreateDeviceResources(HWND m_hwnd)
{
	HRESULT hr = S_OK;

	if (!m_pRenderTarget)
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
			&m_pRenderTarget
		);


		if (SUCCEEDED(hr))
		{
			// Create a gray brush.
			hr = m_pRenderTarget->CreateSolidColorBrush(
				D2D1::ColorF(D2D1::ColorF::LightSlateGray),
				&colorBrush
			);
		}
	}

	return hr;
}

HRESULT Renderer::OnRender(HWND m_hwnd, GameObject* gameObject)
{
	HRESULT hr = S_OK;

	hr = CreateDeviceResources(m_hwnd);
	if (SUCCEEDED(hr))
	{
		m_pRenderTarget->BeginDraw();

		m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

		m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

		gameObject->OnRender(this);

		hr = m_pRenderTarget->EndDraw();
	}

	if (hr == D2DERR_RECREATE_TARGET)
	{
		hr = S_OK;
		DiscardDeviceResources();
	}

	return hr;
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
		m_pRenderTarget->FillRectangle(&rectangle, colorBrush);
	}
	else {
		m_pRenderTarget->DrawRectangle(&rectangle, colorBrush, PixelsToDipsX(strokeWith));
	}
}

void Renderer::RenderCircle(float posX, float posY, float radiusX, float radiusY, D2D1::ColorF color, bool fill, float strokeWith)
{
	D2D1_ELLIPSE ellipse = D2D1::Ellipse(D2D1::Point2(PixelsToDipsX(posX), PixelsToDipsY(posY)),
		PixelsToDipsX(radiusX),
		PixelsToDipsY(radiusY));

	colorBrush->SetColor(color);
	if (fill) {
		m_pRenderTarget->FillEllipse(&ellipse, colorBrush);
	}
	else {
		m_pRenderTarget->DrawEllipse(&ellipse, colorBrush, PixelsToDipsX(strokeWith));
	}
}

void Renderer::RenderImage(float posX, float posY)
{
	// Create a decoder
	IWICBitmapDecoder *pDecoder = NULL;
	IWICImagingFactory *pIWICFactory;
	HRESULT hr = S_OK;

	hr = pIWICFactory->CreateDecoderFromFilename(
		L"",                      // Image to be decoded
		NULL,                            // Do not prefer a particular vendor
		GENERIC_READ,                    // Desired read access to the file
		WICDecodeMetadataCacheOnDemand,  // Cache metadata when needed
		&pDecoder                        // Pointer to the decoder
	);

	// Retrieve the first frame of the image from the decoder
	IWICBitmapFrameDecode *pFrame = NULL;

	if (SUCCEEDED(hr))
	{
		hr = pDecoder->GetFrame(0, &pFrame);
	}
}



void Renderer::OnResize(UINT width, UINT height)
{
	if (m_pRenderTarget)
	{
		// Note: This method can fail, but it's okay to ignore the
		// error here, because the error will be returned again
		// the next time EndDraw is called.
		m_pRenderTarget->Resize(D2D1::SizeU(width, height));
	}
}

void Renderer::DiscardDeviceResources()
{
	SafeRelease(&m_pRenderTarget);
	SafeRelease(&colorBrush);
}
