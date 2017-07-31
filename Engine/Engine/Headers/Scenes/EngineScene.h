#pragma once
#include "Utils\ArrayList.h"
#include "GameObjects\GameObject.h"
#include "Renderer\Renderer.h"
#include "Managers\CacheManager.h"


class EngineScene {

public:
	EngineScene(XEngine& ref):coreRef(ref)
	{
	}

	~EngineScene() {
		gameObjects.empty();
	}

	void Preload() {}

	virtual void Start() = 0;
	virtual void Update(float deltaTime) {
		for (int i = 0; i < gameObjects.size; i++) {
			gameObjects[i]->Update(deltaTime);
		}

	}
	virtual void OnDestroy() = 0;
	void Render(Renderer &renderer) {
		renderer.OnRenderObject(gameObjects);
	}

public:
	bool pendingActivation = true;
	ArrayList<GameObject*> gameObjects;

protected:
	XEngine& coreRef;
};