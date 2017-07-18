#pragma once
#include "Utils\ArrayList.h"
#include "GameObjects\Sprite.h"
#include "GameObjects\GameObject.h"
#include "Renderer\Renderer.h"
#include "Managers\CacheManager.h"

class EngineScene {

public:
	~EngineScene() {
		gameObjects.empty();
	}

	void Preload() {}
	void Start() {
		Sprite* rect = new Sprite(b2Vec2(50.f, 50.f), CacheManager::GetInstance()->AddImage(L"Resources/Engine.ico"));
		gameObjects.insert(rect);
	}
	void Update(float deltaTime) {
		for (int i = 0; i < gameObjects.size_of_list(); i++) {
			gameObjects[i]->Update(deltaTime);
		}
	}
	void OnDestroy() {}
	void Render(HWND hwnd, Renderer* renderer) {
		for (int i = 0; i < gameObjects.size_of_list(); i++) {
			renderer->OnRender(hwnd, gameObjects[i]);
		}
	}

public:
	ArrayList<GameObject*> gameObjects;
};