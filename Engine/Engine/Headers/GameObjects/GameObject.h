#pragma once
#include "XEngine.h"
#include "Managers\Physics.h"
#include "Component\Transform.h"
#include "Utils\MathUtils.h"
#include "Renderer\Renderer.h"

#include "Component\PhysicsBody.h"
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
		coreRef(ref),
		rigidBody(NULL)
	{
		transform = AddComponent<Transform>(false, false);
		transform->position = spawn_position;
		transform->scale = Vector2(1.f, 1.f);
		anchor.x = 0.0L;
		anchor.y = 0.0L;
		bounds = Vector2(0.f, 0.f);
		isPendingDestroy = false;
	}

	~GameObject()
	{
		DeleteComponents();
	}

	virtual void OnRender(Renderer &renderer);
	virtual void Update(float deltaTime) {
		std::map<std::type_index, Component*>::reverse_iterator it = componentsMap.rbegin();
		if (rigidBody != NULL) {
			Vector2 bodyPos = rigidBody->GetPosition();
			transform->position = bodyPos;
			transform->rotation.angles = rigidBody->GetRotation();
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
		T* newComponent = new T(this, &coreRef);
		componentsMap[typeid(T)] = newComponent;
		newComponent->mustUpdate = mustUpdate;
		newComponent->mustRender = mustRender;
		if (typeid(T) == typeid(PhysicsBody)) {
			rigidBody = reinterpret_cast<PhysicsBody*>(newComponent);
			anchor = Vector2(bounds.x / 2.f, bounds.y / 2.f);
		}
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
		if (rigidBody != NULL) {
			rigidBody->Destroy();
		}
		rigidBody = NULL;
	}
public:
	Vector2 anchor;
	Vector2 bounds;
	PhysicsBody* rigidBody = NULL;
	GameObject* parent = NULL;
	float alpha = 1.0;
	bool isPendingDestroy = false;
protected:
	Transform* transform;
	XEngine& coreRef;
	std::map<std::type_index, Component*> componentsMap;
private:
	int test = 54;
};
