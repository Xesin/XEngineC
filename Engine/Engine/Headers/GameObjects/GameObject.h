#pragma once
#include "XEngine.h"
#include "Managers\Physics.h"
#include "Component\Transform.h"
#include "Utils\MathUtils.h"
#include "Renderer\Renderer.h"
#include "Box2D\Common\b2Math.h"
#include "Box2D\Box2D.h"

#include <d2d1.h>
#include <math.h>
#include <map>
#include <typeindex>
#define _USE_MATH_DEFINES

class Renderer;
class Transform;

class GameObject {
public:
	GameObject(Vector2 spawn_position, XEngine& ref) : 
		coreRef(ref)
	{
		transform = AddComponent<Transform>(false, false);
		transform->position = spawn_position;
		scale.width = 1.f;
		scale.height = 1.f;
		anchor.x = 0.0L;
		anchor.y = 0.0L;
		isPendingDestroy = false;
	}

	~GameObject()
	{
		DeleteComponents();
		if (rigidBody != NULL) {
			coreRef.physics->DestroyBody(rigidBody);
			rigidBody = NULL;
		}
	}

	virtual void OnRender(Renderer &renderer);
	virtual void Update(float deltaTime) {
		std::map<std::type_index, Component*>::reverse_iterator it = componentsMap.rbegin();
		if (rigidBody != NULL) {
			b2Vec2 bodyPos = rigidBody->GetPosition();
			transform->position = Renderer::WorldToScreenPixels(Vector2(bodyPos.x, bodyPos.y));
			transform->rotation.angles = RADS_TO_DEGREES(rigidBody->GetAngle());
		}
		for (it; it != componentsMap.rend(); ++it) {
			Component* componentToCheck = it->second;
			if (componentToCheck->markedToDestroy) {
				componentsMap.erase(it->first);
				delete componentToCheck;
			}
			else if (componentToCheck->mustUpdate) {
				componentToCheck->Update();
			}
		}
	}

	inline void SetParent(GameObject* newParent) {
		parent = newParent;
	}

	Transform& GetTransform() {
		return *transform;
	}

	virtual void SetPhysics(bool active, PhysicBodyType bodyType = PhysicBodyType::Static, float32 friction = 1.0f, bool isSensor = false) {}

	inline void WorldTransform(Transform* outTransform) {
		outTransform->position = transform->position;
		outTransform->rotation = transform->rotation;
		outTransform->scale = transform->scale;
		if (parent != NULL) {
			Transform parentTransform;
			transform->parent->WorldTransform(&parentTransform);
			outTransform->position += parentTransform.position;
			outTransform->rotation += parentTransform.rotation;
			outTransform->scale += parentTransform.scale;
		}
	}

	inline void Destroy() {
		isPendingDestroy = true;
		if (rigidBody != NULL) {
			rigidBody->GetWorld()->DestroyBody(rigidBody);
			rigidBody = NULL;
		}

		std::map<std::type_index, Component*>::reverse_iterator it = componentsMap.rbegin();
		for (it; it != componentsMap.rend(); ++it) {
			Component* componentToCheck = it->second;
			componentToCheck->Destroy();
		}
	}

	inline void DeleteComponents() {
		std::map<std::type_index, Component*>::reverse_iterator it = componentsMap.rbegin();
		while (it != componentsMap.rend()) {
			Component* componentToCheck = it->second;
			componentsMap.erase(it->first);
			delete componentToCheck;
		}
	}

	inline bool IsValid() {
		return test == 54;
	}

	template <class T>
	T* AddComponent(bool mustUpdate = true, bool mustRender = false) {
		T* newComponent = new T(this);
		componentsMap[typeid(T)] = newComponent;
		newComponent->mustUpdate = mustUpdate;
		newComponent->mustRender = mustRender;
		return newComponent;
	}

	template <typename T>
	T* GetComponent() {
		T* component = static_cast<T*>(componentsMap[typeid(T)]);
		return *component;
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
	std::map<std::type_index, Component*> componentsMap;
private:
	int test = 54;
};
