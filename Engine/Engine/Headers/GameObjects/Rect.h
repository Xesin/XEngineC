#pragma once
#include "GameObject.h"
#include <d2d1.h>

class Rect : public GameObject {
public:
	Rect(Vector2 spawn_pos, XEngine& ref, int _width, int _height, D2D1::ColorF color);
	void OnRender(Renderer &renderer);
	virtual void Update(float deltaTime) override;

public:
	bool fill = true;
	float32 strokeWith = 1;
	D2D1::ColorF color = D2D1::ColorF(D2D1::ColorF::Black);
};
