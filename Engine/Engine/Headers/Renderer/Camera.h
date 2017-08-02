#pragma once
#include "Box2D\Common\b2Math.h"

class Camera {
public:
	Camera(b2Vec2 worldBounds);
	void SetWorldBounds(b2Vec2 worldBounds);
public:
	b2Vec2 position;
	b2Vec2 bounds;
};