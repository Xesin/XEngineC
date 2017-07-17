#pragma once
#include <d2d1.h>
#include <d2d1helper.h>
#include "Renderer.h"

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

	void RenderRect(float posX, float posY, float width, float height, D2D1::ColorF color);

	void RenderRect(float posX, float posY, float width, float height, D2D1::ColorF color, bool fill, float strokeWith = 1);

	void RenderCircle(float posX, float posY, float radiusX, float radiusY, D2D1::ColorF color);

	void RenderCircle(float posX, float posY, float radiusX, float radiusY, D2D1::ColorF color, bool fill, float strokeWith = 1);

	// Resize the render target.
	void OnResize(
		UINT width,
		UINT height
	);

private:
	// Initialize device-independent resources.
	HRESULT CreateDeviceIndependentResources();

	// Initialize device-dependent resources.
	HRESULT CreateDeviceResources(HWND m_hwnd);

	// Release device-dependent resource.
	void DiscardDeviceResources();
	

private:
	ID2D1Factory* m_pDirect2dFactory;
	ID2D1HwndRenderTarget* m_pRenderTarget;
	ID2D1SolidColorBrush* colorBrush;
};

