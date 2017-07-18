#include "stdafx.h"
#include "Managers\ScaleManager.h"

ScaleManager::ScaleManager(int initialResX, int initialResY)
{
	originalResX = initialResX;
	originalResY = initialResY;
	aspectRatio = originalResY / (float) originalResX;
}

ScaleManager::~ScaleManager()
{
}

void ScaleManager::OnResize(int newResX)
{
	scaleX = (float) newResX / originalResX;
	scaleY = (float) (newResX * aspectRatio) / originalResY;
}
