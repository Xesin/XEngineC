#pragma once
#include "GameObject.h"
class Rect : public GameObject {
public:
	void OnRender(Renderer* renderer) const;
	void Update(float deltaTime) {
	}
	Rect(b2Vec2 spawn_pos, float32 _width, float32 _height);
public:
	float32 width;
	float32 height;
};
