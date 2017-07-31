#pragma once
#include "GameObject.h"
#include <d2d1.h>

class Rect : public GameObject {
public:
	Rect(b2Vec2 spawn_pos, XEngine& ref, float32 _width, float32 _height, D2D1::ColorF color);
	void OnRender(Renderer &renderer);
	virtual void Update(float deltaTime) override;
	virtual void SetPhysics(bool active, bool dynamic = false) override;
public:
	float32 width;
	float32 height;
	bool fill = true;
	float32 strokeWith = 1;
	D2D1::ColorF color = D2D1::ColorF(D2D1::ColorF::Black);
};
