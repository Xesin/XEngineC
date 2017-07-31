#pragma once
#include "Box2D\Common\b2Math.h"
#include "Box2D\Box2D.h"
#include <d2d1.h>
#include <math.h>
#define _USE_MATH_DEFINES

class Renderer;
class XEngine;

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
	}

	virtual void OnRender(Renderer &renderer) = 0;
	virtual void Update(float deltaTime) = 0;
	inline void SetParent(GameObject* newParent) {
		parent = newParent;
	}

	virtual void SetPhysics(bool active, bool dynamic = false) = 0;

	inline void WorldTransform(b2Transform &outTransform) {
		outTransform.p = transform.p;
		outTransform.q = transform.q;
		if (parent != NULL) {
			outTransform.p += parent->transform.p;
			outTransform.q = b2Mul(outTransform.q, parent->transform.q);
		}
	}

	void SetRotation(Renderer &renderer, int width, int height);

public:
	b2Transform transform;
	GameObject* parent = NULL;
	float alpha = 1.0;
	D2D_SIZE_F scale;
	b2Vec2 anchor;
	b2Body* rigidBody = NULL;
protected:
	XEngine& coreRef;
};
