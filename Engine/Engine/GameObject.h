#pragma once
#include "Box2D\Common\b2Math.h"

class Renderer;

class GameObject {
public:
	GameObject(b2Vec2 spawn_position) {
		transform.p = spawn_position;
	}
	virtual void OnRender(Renderer* renderer) const = 0;
	virtual void Update() = 0;
public:
	b2Transform transform;
};
