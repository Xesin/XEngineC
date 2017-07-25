#include "stdafx.h"
#include "Managers\CacheManager.h"
#include "Renderer\CachedImage.h"

CacheManager* CacheManager::instance = NULL;

CacheManager::CacheManager() {
	cachedImages = ArrayList<CachedImage*>(20);
	CacheManager::instance = NULL;
}

CachedImage* CacheManager::AddImage(LPCWSTR imageSource)
{
	CachedImage* newImage = new CachedImage(imageSource);
	cachedImages.insert(newImage);
	return newImage;
}

void CacheManager::FlushCache()
{
	cachedImages.empty();
}

void CacheManager::RefreshCache()
{
	for (int i = 0; i < cachedImages.size; i++) {
		cachedImages[i]->OnRecreateResources();
	}
}

CacheManager* CacheManager::GetInstance() {
	if (!CacheManager::instance) {
		CacheManager::instance = new CacheManager();
	}

	return CacheManager::instance;
}