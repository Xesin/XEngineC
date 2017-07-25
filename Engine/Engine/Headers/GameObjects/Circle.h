#pragma once

#include "GameObject.h"
class Circle : public GameObject {
public:
	void OnRender(Renderer &renderer);
	virtual void Update(float deltaTime) override {};
	Circle(b2Vec2 spawn_pos, float32 _radiusX, float32 _radiusY, D2D1::ColorF color);
public:
	float32 radiusX = 1;
	float32 radiusY = 1;
	D2D1::ColorF color = D2D1::ColorF::Black;
	bool fill = true;
	float32 strokeWith = 1;
};
