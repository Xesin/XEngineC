#pragma once
#include "Managers\Physics.h"
#include "XEngine.h"
#include "Box2D\Common\b2Math.h"
#include "Box2D\Box2D.h"
#include <d2d1.h>
#include <math.h>
#define _USE_MATH_DEFINES

class Renderer;
 
class GameObject {
public:
	GameObject(b2Vec2 spawn_position, XEngine& ref) 
		: coreRef(ref)
	{
		transform.p = spawn_position;
		transform.q.SetIdentity();
		scale.width = 1.f;
		scale.height = 1.f;
		anchor.x = 0.0L;
		anchor.y = 0.0L;
		isPendingDestroy = false;
	}

	virtual void OnRender(Renderer &renderer) = 0;
	virtual void Update(float deltaTime) = 0;
	inline void SetParent(GameObject* newParent) {
		parent = newParent;
	}

	virtual void SetPhysics(bool active, bool dynamic = false, float32 friction = 1.0f) = 0;

	inline void WorldTransform(b2Transform &outTransform) {
		outTransform.p = transform.p;
		outTransform.q = transform.q;
		if (parent != NULL) {
			outTransform.p += parent->transform.p;
			outTransform.q = b2Mul(outTransform.q, parent->transform.q);
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
		coreRef.physics->world.DestroyBody(rigidBody);
		rigidBody = NULL;
	}
public:
	b2Transform transform;
	b2Vec2 anchor;
	b2Body* rigidBody = NULL;
	GameObject* parent = NULL;
	D2D_SIZE_F scale;
	float alpha = 1.0;
	bool isPendingDestroy = false;
protected:
	XEngine& coreRef;
private:
	int test = 54;
};
