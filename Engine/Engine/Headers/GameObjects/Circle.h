#pragma once

#include "GameObject.h"
class Circle : public GameObject {
public:
	Circle(b2Vec2 spawn_pos, XEngine& ref, float32 _radiusX, float32 _radiusY, D2D1::ColorF color);
	void OnRender(Renderer &renderer);
	virtual void Update(float deltaTime) override {};
	void SetPhysics(bool active, bool dynamic = false, float32 friction = 1.0f, float32 radius = 1.0f);
public:
	virtual void SetPhysics(bool active, bool dynamic = false, float32 friction = 1.0f) override {}
	float32 radiusX = 1;
	float32 radiusY = 1;
	D2D1::ColorF color = D2D1::ColorF::Black;
	bool fill = true;
	float32 strokeWith = 1;
};
