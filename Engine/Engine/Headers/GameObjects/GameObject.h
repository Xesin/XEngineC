#pragma once
#include "Box2D\Common\b2Math.h"
#include <d2d1.h>

class Renderer;

class GameObject {
public:
	GameObject(b2Vec2 spawn_position) {
		transform.p = spawn_position;
		transform.q.SetIdentity();
	}

	virtual void OnRender(Renderer* renderer) = 0;
	virtual void Update(float deltaTime) = 0;
	void SetParent(GameObject* newParent) {
		parent = newParent;
	}

	void WorldTransform(b2Transform* outTransform) {
		outTransform->p = transform.p;
		outTransform->q = transform.q;
		if (parent != NULL) {
			outTransform->p += parent->transform.p;
			outTransform->q = b2Mul(outTransform->q, parent->transform.q);
		}
	}
public:
	b2Transform transform;
	GameObject* parent = NULL;
	float alpha = 1.0;
};
