#pragma once
#include <d2d1.h>
#include <d2d1helper.h>
#include "Utils\ArrayList.h"

class CachedImage;

class CacheManager {
public:
	CachedImage* AddImage(LPCWSTR imageSource);
	void FlushCache();
	void RefreshCache();
	static CacheManager* GetInstance();

private:
	CacheManager();

private:
	static CacheManager* instance;
	ArrayList<CachedImage*> cachedImages;
};