#pragma once
#include "Box2D\Common\b2Math.h"
#include "Box2D\Box2D.h"

class Renderer;

enum class PhysicShape {
	Circle,
	Box,
	Edge
};

enum class PhysicBodyType {
	Static,
	Kinematic,
	Dynamic
};

class Physics {
public:
	Physics::Physics(Renderer& ref);
	void Update(float32 deltaTime);
	void DestroyBody(b2Body* bodyToDestroy);
	b2Body* CreateBoxBody(b2Vec2 center, b2Vec2 bounds, float32 density, float32 friction, PhysicBodyType bodyType);
	b2Body* CreateCircleBody(b2Vec2 center, float32 radius, float32 density, float32 friction, PhysicBodyType bodyType);

	void AddBoxFixture(b2Body* body, b2Vec2 center, b2Vec2 bounds, float32 density, float32 friction);
	void AddCircleFixture(b2Body* body, b2Vec2 center, float32 radius, float32 density, float32 friction);
	void AddEdgeFixture(b2Body* body, b2Vec2 p1, b2Vec2 p2, float32 density, float32 friction);

	void SetGravity(b2Vec2 newGravity);
	void DrawDebug();

	b2DistanceJoint* MakeFixedDistanceJoint(b2Body* body1, b2Body* body2, bool collideConnected);
private:
	b2World& world = b2World(gravity);
	b2Vec2 gravity = b2Vec2(0.0f, -15.0f);
public:
	bool isDebug = false;
};