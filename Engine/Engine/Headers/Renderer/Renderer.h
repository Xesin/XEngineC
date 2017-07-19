#pragma once
#include <d2d1.h>
#include <d2d1helper.h>
#include <wincodec.h>

template<class Interface>
inline void SafeRelease(
	Interface **ppInterfaceToRelease
)
{
	if (*ppInterfaceToRelease != NULL)
	{
		(*ppInterfaceToRelease)->Release();

		(*ppInterfaceToRelease) = NULL;
	}
}

class GameObject;
class CachedImage;
class ScaleManager;

class Renderer
{
public:
	Renderer();
	~Renderer();

	// Register the window class and call methods for instantiating drawing resources
	HRESULT Initialize();

	void GetDesktopDpi(FLOAT* dpiX, FLOAT* dpiY);

	// Draw content.
	HRESULT OnRender(HWND m_hwnd, GameObject* gameObject);

	// InitializeRender
	void PreRender();

	// Ends the render
	void EndRender();

	void RenderRect(float posX, float posY, float width, float height, D2D1::ColorF color, bool fill, float strokeWith = 1);

	void RenderCircle(float posX, float posY, float radiusX, float radiusY, D2D1::ColorF color, bool fill, float strokeWith = 1);

	void RenderImage(float posX, float posY, CachedImage* imageToRender, int frameColumn, int frameRow, int frame, int frameWidth, int frameHeight);
	// Resize the render target.
	void OnResize(
		UINT width,
		UINT height
	);

	static IWICImagingFactory* GetIwicFactory();

	static HRESULT CreateIwicFactory();

	static ID2D1HwndRenderTarget* GetRenderTarget();

	// Initialize device-dependent resources.
	HRESULT CreateDeviceResources(HWND m_hwnd);

private:
	// Initialize device-independent resources.
	HRESULT CreateDeviceIndependentResources();

	// Release device-dependent resource.
	void DiscardDeviceResources();

	template <typename T>
	float PixelsToDipsX(T x)
	{
		return (static_cast<float>(x) / DPIScaleX) * scaleManager->scaleX;
	}

	template <typename T>
	float PixelsToDipsY(T y)
	{
		return (static_cast<float>(y) / DPIScaleY) * scaleManager->scaleY;
	}
	
public:
	static IWICImagingFactory *wicFactory;
	static ID2D1HwndRenderTarget* m_pRenderTarget;
private:
	
	ID2D1Factory* m_pDirect2dFactory;
	ID2D1SolidColorBrush* colorBrush;
	float DPIScaleX = 1.f;
	float DPIScaleY = 1.f;
	ScaleManager* scaleManager;
};



