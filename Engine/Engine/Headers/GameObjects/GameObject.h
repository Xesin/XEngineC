#pragma once
#include "XEngine.h"
#include "Managers\Physics.h"
#include "Component\Transform.h"
#include "Utils\MathUtils.h"

#include "Box2D\Common\b2Math.h"
#include "Box2D\Box2D.h"

#include <d2d1.h>
#include <math.h>
#define _USE_MATH_DEFINES

class Renderer;
class Transform;

class GameObject {
public:
	GameObject(Vector2 spawn_position, XEngine& ref) : 
		coreRef(ref)
	{
		transform = new Transform(this);
		transform->position = spawn_position;
		scale.width = 1.f;
		scale.height = 1.f;
		anchor.x = 0.0L;
		anchor.y = 0.0L;
		isPendingDestroy = false;
	}

	virtual void OnRender(Renderer &renderer);
	virtual void Update(float deltaTime) = 0;

	inline void SetParent(GameObject* newParent) {
		parent = newParent;
	}

	Transform& GetTransform() {
		return *transform;
	}

	virtual void SetPhysics(bool active, PhysicBodyType bodyType = PhysicBodyType::Static, float32 friction = 1.0f, bool isSensor = false) = 0;

	inline void WorldTransform(Transform* outTransform) {
		outTransform->position = transform->position;
		outTransform->rotation = transform->rotation;
		if (parent != NULL) {
			Transform parentTransform;
			transform->parent->WorldTransform(&parentTransform);
			outTransform->position += parentTransform.position;
			outTransform->rotation += parentTransform.rotation;
		}
	}

	inline void Destroy() {
		isPendingDestroy = true;
		if (rigidBody != NULL) {
			rigidBody->GetWorld()->DestroyBody(rigidBody);
			rigidBody = NULL;
		}
	}

	inline bool IsValid() {
		return test == 54;
	}
protected:
	virtual void SetTransform(Renderer &renderer, int width, int height);
	inline void DestroyBody() {
		coreRef.physics->DestroyBody(rigidBody);
		rigidBody = NULL;
	}
public:
	b2Vec2 anchor;
	b2Body* rigidBody = NULL;
	GameObject* parent = NULL;
	D2D_SIZE_F scale;
	float alpha = 1.0;
	bool isPendingDestroy = false;
protected:
	Transform* transform;
	XEngine& coreRef;
private:
	int test = 54;
};
