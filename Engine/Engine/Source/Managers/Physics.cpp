#include "Managers\Physics.h"
#include "Renderer\Renderer.h"
#include "Box2D\Box2D.h"


Physics::Physics(Renderer& ref)
{
	gravity = Vector2(0.0f, -25.0f);
	world = *(new b2World(b2Vec2(gravity.x, gravity.y)));
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
	world.Step(deltaTime, 6, 6);
}

void Physics::DestroyBody(b2Body * bodyToDestroy)
{
	world.DestroyBody(bodyToDestroy);
}

b2Body * Physics::CreateBoxBody(Vector2 center, Vector2 bounds, float32 density, float32 friction, PhysicBodyType bodyType, bool isSensor)
{
	b2BodyDef bodyDef;

	b2Body* rigidBody;
	b2Vec2 centerInUnits = Renderer::PixelsToWorldUnits(b2Vec2(center.x, center.y));
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
	AddBoxFixture(rigidBody, Vector2(.0f, .0f), bounds, density, friction, isSensor);

	return rigidBody;
}

b2Body * Physics::CreateCircleBody(Vector2 center, float32 radius, float32 density, float32 friction, PhysicBodyType bodyType, bool isSensor)
{
	
	b2BodyDef bodyDef;

	b2Body* rigidBody;
	b2Vec2 centerInUnits = Renderer::PixelsToWorldUnits(b2Vec2(center.x, center.y));
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

	AddCircleFixture(rigidBody, Vector2(.0f, .0f), radius, density, friction, isSensor);

	return rigidBody;
}

b2Body * Physics::CreateEgeBody(Vector2 center, Vector2 p1, Vector2 p2, float32 density, float32 friction, PhysicBodyType bodyType, bool isSensor)
{
	b2BodyDef bodyDef;

	b2Body* rigidBody;
	b2Vec2 centerInUnits = Renderer::PixelsToWorldUnits(b2Vec2(center.x, center.y));
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

b2Body * Physics::CreateEmptyBody(Vector2 center)
{
	b2BodyDef bodyDef;

	b2Body* rigidBody;
	b2Vec2 centerInUnits = Renderer::PixelsToWorldUnits(b2Vec2(center.x, center.y));
	bodyDef.position.Set(centerInUnits.x, centerInUnits.y);

	rigidBody = world.CreateBody(&bodyDef);
	return rigidBody;
}

void Physics::AddBoxFixture(b2Body* body, Vector2 center, Vector2 bounds, float32 density, float32 friction, bool isSensor)
{
	b2FixtureDef fixtureDef;
	b2PolygonShape boxShape;
	b2Vec2 centerInUnits = Renderer::PixelsToWorldUnits(b2Vec2(center.x, center.y));
	Vector2 worldUnitsBound = Renderer::PixelsToWorldUnits(bounds);
	boxShape.SetAsBox(worldUnitsBound.x - 0.01f, worldUnitsBound.y - 0.01f, centerInUnits, 0.0f);

	fixtureDef.shape = &boxShape;
	fixtureDef.density = density;
	fixtureDef.friction = friction;
	fixtureDef.isSensor = isSensor;

	body->CreateFixture(&fixtureDef);
}

void Physics::AddCircleFixture(b2Body* body, Vector2 center, float32 radius, float32 density, float32 friction, bool isSensor)
{
	b2FixtureDef fixtureDef;
	b2CircleShape circShape;

	circShape.m_radius = Renderer::PixelsToWorldUnits(radius);
	b2Vec2 centerInUnits = b2Vec2(center.x, center.y);
	circShape.m_p = Renderer::PixelsToWorldUnits(centerInUnits);

	fixtureDef.shape = &circShape;
	fixtureDef.density = density;
	fixtureDef.friction = friction;
	fixtureDef.isSensor = isSensor;

	body->CreateFixture(&fixtureDef);
}

void Physics::AddEdgeFixture(b2Body * body, Vector2 p1, Vector2 p2, float32 density, float32 friction, bool isSensor)
{
	b2FixtureDef fixtureDef;
	b2EdgeShape edgeShape;

	edgeShape.Set(Renderer::PixelsToWorldUnits(b2Vec2(p1.x, p1.y)), Renderer::PixelsToWorldUnits(b2Vec2(p2.x, p2.y)));

	fixtureDef.shape = &edgeShape;
	fixtureDef.density = density;
	fixtureDef.friction = friction;
	fixtureDef.isSensor = isSensor;

	body->CreateFixture(&fixtureDef);
}

void Physics::SetGravity(Vector2 newGravity)
{
	world.SetGravity(b2Vec2(newGravity.x, newGravity.y));
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
