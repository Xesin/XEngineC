#pragma once
#include "Component\Component.h"


class b2Body;
enum class PhysicBodyType;
struct Vector2;
struct b2MassData;

class PhysicsBody : public Component {

public:
	PhysicsBody(GameObject* parent, XEngine* engineRef);

	void AddCircleShape(Vector2 center, float radius, bool isSensor, float friction);

	void AddSquareShape(Vector2 center, Vector2 bounds, bool isSensor, float friction);

	void AddEdgeShape(Vector2 p1, Vector2 p2, bool isSensor, float friction);

	Vector2 GetPosition();

	float GetRotation();

	void SetType(PhysicBodyType bodyType);

	void Translate(Vector2 position);

	void Rotate(float rotarion);

	void ApplyForce(Vector2 force, Vector2 position);

	void SetLinearVelocity(Vector2 velocity);

	void SetGravityScale(float gravityScale);

	Vector2 GetLinearVelocity();

	Vector2 GetWorldCenter();

	void GetMassData(b2MassData *massData);

	void SetMassData(b2MassData *massData);

	void SetFixedRotation(bool fixedRotation);

	virtual void OnDestroy() override;

	b2Body* rigidBody;
private:
};