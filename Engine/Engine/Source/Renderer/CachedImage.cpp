#include "stdafx.h"
#include "Renderer\CachedImage.h"
#include <wincodec.h>

#include "Renderer\Renderer.h"

CachedImage::CachedImage(LPCWSTR _imageName)
{
	// Create a decoder
	imageName = _imageName;
	Generate2D1Bitmap(imageName);
}

CachedImage::~CachedImage()
{
	SafeRelease(&cachedBitmap);
}

void CachedImage::OnRecreateResources()
{
	SafeRelease(&cachedBitmap);
	Generate2D1Bitmap(imageName);
}

ID2D1Bitmap* CachedImage::Get2D2Bitmap()
{
	return cachedBitmap;
}

HRESULT CachedImage::Generate2D1Bitmap(LPCWSTR imageName)
{
	IWICBitmapDecoder *pDecoder = NULL;
	HRESULT hr = S_OK;

	hr = Renderer::GetIwicFactory()->CreateDecoderFromFilename(
		imageName,                      // Image to be decoded
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

	IWICFormatConverter* m_pConvertedSourceBitmap;

	// Format convert the frame to 32bppPBGRA
	if (SUCCEEDED(hr))
	{
		//SafeRelease(&m_pConvertedSourceBitmap);
		hr = Renderer::GetIwicFactory()->CreateFormatConverter(&m_pConvertedSourceBitmap);
	}

	if (SUCCEEDED(hr))
	{
		hr = m_pConvertedSourceBitmap->Initialize(
			pFrame,                          // Input bitmap to convert
			GUID_WICPixelFormat32bppPBGRA,   // Destination pixel format
			WICBitmapDitherTypeNone,         // Specified dither pattern
			NULL,                            // Specify a particular palette 
			0.f,                             // Alpha threshold
			WICBitmapPaletteTypeCustom       // Palette translation type
		);
	}

	if (SUCCEEDED(hr))
	{
		// D2DBitmap may have been released due to device loss. 
		// If so, re-create it from the source bitmap
		ID2D1Bitmap* cachedBitmap;
		if (m_pConvertedSourceBitmap)
		{
			Renderer::GetRenderTarget()->CreateBitmapFromWicBitmap(m_pConvertedSourceBitmap, NULL, &cachedBitmap);
			this->cachedBitmap = cachedBitmap;
		}
		SafeRelease(&m_pConvertedSourceBitmap);
	}

	
	SafeRelease(&pFrame);
	SafeRelease(&pDecoder);

	return hr;
}
