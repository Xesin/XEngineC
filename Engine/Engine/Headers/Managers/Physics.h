#pragma once
#include "Box2D\Common\b2Math.h"
#include "Box2D\Box2D.h"

class Renderer;

enum class PhysicShape {
	Circle,
	Box,
	Edge
};

class Physics {
public:
	Physics::Physics(Renderer& ref);
	void Update(float32 deltaTime);
private:
public:
	b2Vec2 gravity = b2Vec2(0.0f, -15.0f);
	b2World& world = b2World(gravity);
	bool drawDebug = false;
};