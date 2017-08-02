#include "Managers\Physics.h"
#include "Renderer\Renderer.h"
#include "Box2D\Box2D.h"


Physics::Physics(Renderer& ref)
{
	gravity = b2Vec2(0.0f, -25.0f);
	world = *(new b2World(gravity));
	world.SetDebugDraw(&ref);
	uint32 flags = 0;
	flags += b2Draw::e_shapeBit;
	flags += b2Draw::e_jointBit;
	flags += b2Draw::e_pairBit;
	flags += b2Draw::e_centerOfMassBit;
	ref.SetFlags(flags);
	isDebug = false;
}

void Physics::Update(float32 deltaTime)
{
	world.Step(deltaTime, 6, 2);
}

void Physics::DestroyBody(b2Body * bodyToDestroy)
{
	world.DestroyBody(bodyToDestroy);
}

b2Body * Physics::CreateBoxBody(b2Vec2 center, b2Vec2 bounds, float32 density, float32 friction, bool dynamic)
{
	b2BodyDef bodyDef;

	b2Body* rigidBody;
	bodyDef.position.Set(center.x, center.y);

	if (dynamic) {
		bodyDef.type = b2_dynamicBody;
	}

	rigidBody = world.CreateBody(&bodyDef);
	AddBoxFixture(rigidBody, b2Vec2(.0f, .0f), bounds, density, friction);

	return rigidBody;
}

b2Body * Physics::CreateCircleBody(b2Vec2 center, float32 radius, float32 density, float32 friction, bool dynamic)
{
	
	b2BodyDef bodyDef;

	b2Body* rigidBody;

	bodyDef.position.Set(center.x, center.y);
	if (dynamic) {
		bodyDef.type = b2_dynamicBody;
	}
	rigidBody = world.CreateBody(&bodyDef);

	AddCircleFixture(rigidBody, b2Vec2(.0f, .0f), radius, density, friction);

	return rigidBody;
}

void Physics::AddBoxFixture(b2Body* body, b2Vec2 center, b2Vec2 bounds, float32 density, float32 friction)
{
	b2FixtureDef fixtureDef;
	b2PolygonShape boxShape;

	boxShape.SetAsBox(bounds.x, bounds.y, center, 0.0f);

	fixtureDef.shape = &boxShape;
	fixtureDef.density = density;
	fixtureDef.friction = friction;

	body->CreateFixture(&fixtureDef);
}

void Physics::AddCircleFixture(b2Body* body, b2Vec2 center, float32 radius, float32 density, float32 friction)
{
	b2FixtureDef fixtureDef;
	b2CircleShape circShape;

	circShape.m_radius = radius;
	circShape.m_p = center;

	fixtureDef.shape = &circShape;
	fixtureDef.density = density;
	fixtureDef.friction = friction;

	body->CreateFixture(&fixtureDef);
}



void Physics::SetGravity(b2Vec2 newGravity)
{
	world.SetGravity(newGravity);
}

void Physics::DrawDebug()
{
	world.DrawDebugData();
}

b2DistanceJoint* Physics::MakeFixedDistanceJoint(b2Body * body1, b2Body * body2, bool collideConnected)
{
	b2DistanceJointDef jointDef;
	jointDef.Initialize(body1, body2, body1->GetWorldCenter(), body2->GetWorldCenter());
	jointDef.collideConnected = collideConnected;

	b2DistanceJoint* joint = (b2DistanceJoint*)world.CreateJoint(&jointDef);

	return joint;
}
