#pragma once
#include "Utils\ArrayList.h"
#include "GameObjects\GameObject.h"
#include "Renderer\Renderer.h"
#include "Managers\CacheManager.h"


class EngineScene {

public:
	~EngineScene() {
		gameObjects.empty();
	}

	void Preload() {}

	void Test(unsigned int keycode) {
		
	}

	virtual void Start() = 0;
	virtual void Update(float deltaTime) {
		for (int i = 0; i < gameObjects.size_of_list(); i++) {
			gameObjects[i]->Update(deltaTime);
		}

	}
	void OnDestroy() {}
	void Render(Renderer* renderer) {
		for (int i = 0; i < gameObjects.size_of_list(); i++) {
			renderer->OnRenderObject(gameObjects[i]);
		}
	}

public:
	ArrayList<GameObject*> gameObjects;
};