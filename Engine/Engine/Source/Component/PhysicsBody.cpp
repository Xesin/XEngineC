#include "Component\PhysicsBody.h"
#include "Component\Transform.h"
#include "GameObjects\GameObject.h"
#include "XEngine.h"
#include "Managers\Physics.h"
#include "Utils\MathUtils.h"

#include "Box2D\Box2D.h"
#include "Box2D\Common\b2Math.h"

PhysicsBody::PhysicsBody(GameObject* parent, XEngine* engineRef) : Component(parent, engineRef) {
	Transform parentTransorm;
	parent->WorldTransform(&parentTransorm);
	rigidBody = engineRef->physics->CreateEmptyBody(parentTransorm.position);
}

void PhysicsBody::AddCircleShape(Vector2 center, float radius, bool isSensor, float friction)
{
	engineRef->physics->AddCircleFixture(rigidBody, center, radius, 1.0, friction, isSensor);
}

void PhysicsBody::AddSquareShape(Vector2 center, Vector2 bounds, bool isSensor, float friction)
{
	engineRef->physics->AddBoxFixture(rigidBody, center, bounds, 1.0, friction, isSensor);
}

void PhysicsBody::AddEdgeShape(Vector2 p1, Vector2 p2, bool isSensor, float friction)
{
	engineRef->physics->AddEdgeFixture(rigidBody, p1, p2, 1.0, friction, isSensor);
}



Vector2 PhysicsBody::GetPosition()
{
	b2Vec2 position = Renderer::WorldToScreenPixels(rigidBody->GetPosition());
	return Vector2(position.x, position.y);
}

float PhysicsBody::GetRotation()
{
	float angles = rigidBody->GetAngle();
	return RADS_TO_DEGREES(angles);
}

void PhysicsBody::SetType(PhysicBodyType bodyType)
{
	switch (bodyType)
	{
	case PhysicBodyType::Static: 
	{
		rigidBody->SetType(b2BodyType::b2_staticBody);
	}
		break;
	case PhysicBodyType::Kinematic:
	{
		rigidBody->SetType(b2BodyType::b2_kinematicBody);
	}
		break;
	case PhysicBodyType::Dynamic:
	{
		rigidBody->SetType(b2BodyType::b2_dynamicBody);
	}
		break;
	default:
	{
		rigidBody->SetType(b2BodyType::b2_staticBody);
	}
		break;
	}
}

void PhysicsBody::Translate(Vector2 position)
{
	Vector2 WorldUnitsPosition = Renderer::PixelsToWorldUnits(position);
	rigidBody->SetTransform(b2Vec2(WorldUnitsPosition.x, WorldUnitsPosition.y), rigidBody->GetAngle());
}

void PhysicsBody::Rotate(float rotation)
{
	rigidBody->SetTransform(rigidBody->GetPosition(), DEGREES_TO_RADS(rotation));
}

void PhysicsBody::ApplyForce(Vector2 force, Vector2 position)
{
	rigidBody->ApplyForce(b2Vec2(force.x, force.y), b2Vec2(position.x, position.y), true);
}

void PhysicsBody::SetLinearVelocity(Vector2 velocity)
{
	rigidBody->SetLinearVelocity(Renderer::PixelsToWorldUnits(b2Vec2(velocity.x, velocity.y)));
}

void PhysicsBody::SetGravityScale(float gravityScale)
{
	rigidBody->SetGravityScale(gravityScale);
}

Vector2 PhysicsBody::GetLinearVelocity()
{
	b2Vec2 linearVelocity = Renderer::WorldToScreenPixels(rigidBody->GetLinearVelocity());
	return Vector2(linearVelocity.x, linearVelocity.y);
}

Vector2 PhysicsBody::GetWorldCenter()
{
	b2Vec2 worldCenter = rigidBody->GetWorldCenter();
	return Vector2(worldCenter.x, worldCenter.y);
}

void PhysicsBody::GetMassData(b2MassData * massData)
{
	rigidBody->GetMassData(massData);
}

void PhysicsBody::SetMassData(b2MassData * massData)
{
	rigidBody->SetMassData(massData);
}

void PhysicsBody::SetFixedRotation(bool fixedRotation)
{
	rigidBody->SetFixedRotation(true);
}

void PhysicsBody::OnDestroy()
{
	engineRef->physics->DestroyBody(rigidBody);
}
