#pragma once
#include "GameObject.h"
class Rect : public GameObject {
public:
	void OnRender(Renderer* renderer) const;
	Rect(b2Vec2 spawn_pos) : GameObject(spawn_pos) {

	}

public:
	float x = 0;
};
