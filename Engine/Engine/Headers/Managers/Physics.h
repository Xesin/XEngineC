#pragma once
#include "Box2D\Common\b2Math.h"
#include "Box2D\Box2D.h"
#include "Utils\MathUtils.h"

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
	b2Body* CreateBoxBody(Vector2 center, Vector2 bounds, float32 density, float32 friction, PhysicBodyType bodyType, bool isSensor = false);
	b2Body* CreateCircleBody(Vector2 center, float32 radius, float32 density, float32 friction, PhysicBodyType bodyType, bool isSensor = false);
	b2Body* CreateEgeBody(Vector2 center, Vector2 p1, Vector2 p2, float32 density, float32 friction, PhysicBodyType bodyType, bool isSensor = false);
	b2Body* CreateEmptyBody(Vector2 center);
	void AddBoxFixture(b2Body* body, Vector2 center, Vector2 bounds, float32 density, float32 friction, bool isSensor = false);
	void AddCircleFixture(b2Body* body, Vector2 center, float32 radius, float32 density, float32 friction, bool isSensor = false);
	void AddEdgeFixture(b2Body* body, Vector2 p1, Vector2 p2, float32 density, float32 friction, bool isSensor = false);

	void SetGravity(Vector2 newGravity);
	void DrawDebug();

	b2DistanceJoint* MakeFixedDistanceJoint(b2Body* body1, b2Body* body2, bool collideConnected);
private:
	b2World& world = b2World(b2Vec2(0.f, -10.f));
	Vector2 gravity = Vector2(0.0f, -15.0f);
public:
	bool isDebug = false;
};