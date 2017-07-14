#pragma once
#include "Box2D\Common\b2Math.h"

class Renderer;

class GameObject {
public:
	GameObject(b2Vec2 spawn_position) {
		position = spawn_position;
	}
	virtual void OnRender(Renderer* renderer) const = 0;

public:
	b2Vec2 position;
};
