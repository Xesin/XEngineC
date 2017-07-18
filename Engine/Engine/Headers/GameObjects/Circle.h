#pragma once

#include "GameObject.h"
class Circle : public GameObject {
public:
	void OnRender(Renderer* renderer) const;
	void Update(float deltaTime) {
	}
	Circle(b2Vec2 spawn_pos, float32 _radiusX, float32 _radiusY);
public:
	float32 radiusX = 1;
	float32 radiusY = 1;
	bool fill = true;
	float32 strokeWith = 1;
};
