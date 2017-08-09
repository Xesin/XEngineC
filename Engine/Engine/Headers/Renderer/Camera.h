#pragma once
#include "Utils\MathUtils.h"
struct Vector2;

class Camera {
public:
	Camera(Vector2 worldBounds);
	void SetWorldBounds(Vector2 worldBounds);
public:
	Vector2 position;
	Vector2 bounds;
};