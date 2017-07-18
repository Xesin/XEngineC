#pragma once

class ScaleManager {
public:
	ScaleManager(int initialResX, int initialResY);
	~ScaleManager();

	void OnResize(int newResX);

public:
	float scaleX = 1.f;
	float scaleY = 1.f;
	float aspectRatio;

private:
	int originalResX;
	int originalResY;
};