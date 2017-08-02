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

b2Body * Physics::CreateBoxBody(b2Vec2 center, b2Vec2 bounds, float32 density, float32 friction, PhysicBodyType bodyType, bool isSensor)
{
	b2BodyDef bodyDef;

	b2Body* rigidBody;
	b2Vec2 centerInUnits = Renderer::PixelsToWorldUnits(center);
	bodyDef.position.Set(centerInUnits.x, centerInUnits.y);

	switch (bodyType)
	{
	case PhysicBodyType::Static:
	{
		bodyDef.type = b2_staticBody;
	}
		break;
	case PhysicBodyType::Kinematic:
	{
		bodyDef.type = b2_kinematicBody;
	}
		break;
	case PhysicBodyType::Dynamic:
	{
		bodyDef.type = b2_dynamicBody;
	}
		break;
	
	default:
		break;
	}

	rigidBody = world.CreateBody(&bodyDef);
	AddBoxFixture(rigidBody, b2Vec2(.0f, .0f), bounds, density, friction, isSensor);

	return rigidBody;
}

b2Body * Physics::CreateCircleBody(b2Vec2 center, float32 radius, float32 density, float32 friction, PhysicBodyType bodyType, bool isSensor)
{
	
	b2BodyDef bodyDef;

	b2Body* rigidBody;
	b2Vec2 centerInUnits = Renderer::PixelsToWorldUnits(center);
	bodyDef.position.Set(centerInUnits.x, centerInUnits.y);
	switch (bodyType)
	{
	case PhysicBodyType::Static:
	{
		bodyDef.type = b2_staticBody;
	}
	break;
	case PhysicBodyType::Kinematic:
	{
		bodyDef.type = b2_kinematicBody;
	}
	break;
	case PhysicBodyType::Dynamic:
	{
		bodyDef.type = b2_dynamicBody;
	}
	break;

	default:
		break;
	}
	rigidBody = world.CreateBody(&bodyDef);

	AddCircleFixture(rigidBody, b2Vec2(.0f, .0f), radius, density, friction, isSensor);

	return rigidBody;
}

b2Body * Physics::CreateEgeBody(b2Vec2 center, b2Vec2 p1, b2Vec2 p2, float32 density, float32 friction, PhysicBodyType bodyType, bool isSensor)
{
	b2BodyDef bodyDef;

	b2Body* rigidBody;
	b2Vec2 centerInUnits = Renderer::PixelsToWorldUnits(center);
	bodyDef.position.Set(centerInUnits.x, centerInUnits.y);

	switch (bodyType)
	{
	case PhysicBodyType::Static:
	{
		bodyDef.type = b2_staticBody;
	}
	break;
	case PhysicBodyType::Kinematic:
	{
		bodyDef.type = b2_kinematicBody;
	}
	break;
	case PhysicBodyType::Dynamic:
	{
		bodyDef.type = b2_dynamicBody;
	}
	break;

	default:
		break;
	}

	rigidBody = world.CreateBody(&bodyDef);
	AddEdgeFixture(rigidBody, p1, p2, density, friction, isSensor);

	return rigidBody;
}

void Physics::AddBoxFixture(b2Body* body, b2Vec2 center, b2Vec2 bounds, float32 density, float32 friction, bool isSensor)
{
	b2FixtureDef fixtureDef;
	b2PolygonShape boxShape;

	boxShape.SetAsBox(Renderer::PixelsToWorldUnits(bounds.x) - 0.01f, Renderer::PixelsToWorldUnits(bounds.y) - 0.01f, Renderer::PixelsToWorldUnits(center), 0.0f);

	fixtureDef.shape = &boxShape;
	fixtureDef.density = density;
	fixtureDef.friction = friction;
	fixtureDef.isSensor = isSensor;

	body->CreateFixture(&fixtureDef);
}

void Physics::AddCircleFixture(b2Body* body, b2Vec2 center, float32 radius, float32 density, float32 friction, bool isSensor)
{
	b2FixtureDef fixtureDef;
	b2CircleShape circShape;

	circShape.m_radius = Renderer::PixelsToWorldUnits(radius) - 0.01f;
	circShape.m_p = Renderer::PixelsToWorldUnits(center);

	fixtureDef.shape = &circShape;
	fixtureDef.density = density;
	fixtureDef.friction = friction;
	fixtureDef.isSensor = isSensor;

	body->CreateFixture(&fixtureDef);
}

void Physics::AddEdgeFixture(b2Body * body, b2Vec2 p1, b2Vec2 p2, float32 density, float32 friction, bool isSensor)
{
	b2FixtureDef fixtureDef;
	b2EdgeShape edgeShape;
	edgeShape.Set(Renderer::PixelsToWorldUnits(p1), Renderer::PixelsToWorldUnits(p2));

	fixtureDef.shape = &edgeShape;
	fixtureDef.density = density;
	fixtureDef.friction = friction;
	fixtureDef.isSensor = isSensor;

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
