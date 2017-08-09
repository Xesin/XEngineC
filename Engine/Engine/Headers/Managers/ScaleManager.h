#pragma once
#include "Utils\MathUtils.h"

class ScaleManager {
public:
	ScaleManager(int initialResX, int initialResY);
	~ScaleManager();

	void OnResize(int newResX);

public:
	float renderTargetScaleX = 1.f;
	float renderTargetScaleY = 1.f;
	Vector2 gameScale = Vector2(1.f, 1.f);
	float aspectRatio;

private:
	int originalResX;
	int originalResY;
};