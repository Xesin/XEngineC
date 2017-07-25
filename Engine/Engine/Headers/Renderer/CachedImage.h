#pragma once
#include <d2d1.h>
#include <d2d1helper.h>

class CachedImage {

public:
	CachedImage();
	CachedImage(LPCWSTR imageName);
	~CachedImage();

	void OnRecreateResources();

	ID2D1Bitmap* Get2D2Bitmap();

private:
	HRESULT Generate2D1Bitmap(LPCWSTR imageName);

private:
	ID2D1Bitmap *cachedBitmap;
	LPCWSTR imageName;
};
