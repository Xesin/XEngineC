#pragma once
#include <d2d1.h>
#include <d2d1helper.h>
#include <wincodec.h>
#include <algorithm>
#include "Utils\Delegates\DelegatesSettings.h"
#include "Utils\MathUtils.h"
#include "Utils\ArrayList.h"
#include "Box2D\Common\b2Draw.h"

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
class Camera;

class Renderer : public b2Draw
{
public:
	Renderer();
	~Renderer();

	// Register the window class and call methods for instantiating drawing resources
	HRESULT Initialize(Camera* newCamera);

	// Initialize device-dependent resources.
	HRESULT CreateDeviceResources(HWND m_hwnd);

	void SetClearColor(D2D1::ColorF color);

	void GetDesktopDpi(FLOAT* dpiX, FLOAT* dpiY);

	static IWICImagingFactory* GetIwicFactory();

	static HRESULT CreateIwicFactory();

	static ID2D1HwndRenderTarget* GetRenderTarget();

	// Draw content.
	void OnRenderGroup(ArrayList<GameObject*> &gameObject);

	// InitializeRender
	HRESULT PreRender(HWND m_hwnd);

	// Ends the render
	void EndRender();

	void SetTransform(D2D1::Matrix3x2F transform);

	void SetTransform(Vector2 position, Vector2 bounds, Vector2 scale, Vector2 anchor, float angle);

	void RenderRect(float posX, float posY, int width, int height, D2D1::ColorF color, Vector2 scale, bool fill = true, float strokeWith = 1);

	void RenderCircle(float posX, float posY, float radiusX, float radiusY, D2D1::ColorF color, Vector2 scale, bool fill = true, float strokeWith = 1);

	void RenderImage(float posX, float posY, CachedImage &imageToRender, int frameColumn, int frameRow, int frame, int frameWidth, int frameHeight, Vector2 scale);

	void RenderTilledImage(Vector2 position, CachedImage &imageToRender, int frameColumn, int frameRow, int frame, Vector2 frameSize, Vector2 tileScroll, Vector2 tileSize, Vector2 anchor);

	void CreateTilledBitmapBrush(CachedImage & imageToRender, int frameColumn, int frameRow, int frame, Vector2 frameSize, ID2D1BitmapBrush **m_pBitmapBrush, Vector2 tileScroll);
	// Resize the render target.
	void OnResize(
		UINT width,
		UINT height
	);

	void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override;

	void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override;

	void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color) override;

	void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color) override;

	void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) override;

	void DrawTransform(const b2Transform& xf) override;

	void DrawPoint(const b2Vec2& p, float32 size, const b2Color& color) override;

	static b2Vec2 WorldToScreenPixels(b2Vec2 worldUnit);
	static b2Vec2 PixelsToWorldUnits(b2Vec2 screenPixel);

	static Vector2 WorldToScreenPixels(Vector2 worldUnit);
	static Vector2 PixelsToWorldUnits(Vector2 screenPixel);

	static float WorldToScreenPixels(float worldUnit);
	static float PixelsToWorldUnits(float screenPixel);

private:
	// Initialize device-independent resources.
	HRESULT CreateDeviceIndependentResources();

	// Release device-dependent resource.
	void DiscardDeviceResources();

	template <typename T>
	float PixelsToDipsX(T x)
	{
		return (static_cast<float>(x) / DPIScaleX) * scaleManager->renderTargetScaleX;
	}

	template <typename T>
	float PixelsToDipsY(T y)
	{
		return (static_cast<float>(y) / DPIScaleY) * scaleManager->renderTargetScaleY;
	}

	Vector2 GetCameraPos();
	
public:
	static IWICImagingFactory *wicFactory;
	static ID2D1HwndRenderTarget* renderTarget;
	ScaleManager* scaleManager;
	ID2D1BitmapBrush* tilledBrush;
private:
	ID2D1Factory* m_pDirect2dFactory;
	ID2D1SolidColorBrush* colorBrush;
	D2D1::Matrix3x2F canvasScaleMatrix;
	D2D1::Matrix3x2F canvasTranslationMatrix;
	float DPIScaleX = 1.f;
	float DPIScaleY = 1.f;
	Camera* camera;
	D2D1::ColorF clearColor = D2D1::ColorF::Black;
};



