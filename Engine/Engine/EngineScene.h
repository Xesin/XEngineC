#pragma once
#include "ArrayList.h"
#include "Rect.h"
#include "GameObject.h"
#include "Renderer.h"

#define SuperScene EngineScene

class EngineScene {

public:
	~EngineScene() {
		gameObjects.empty();
	}

	void Preload() {}
	void Start() {
		gameObjects.insert(new Rect(b2Vec2(50.f, 40.f), 100, 100));
	}
	void Update() {
		for (int i = 0; i < gameObjects.size_of_list(); i++) {
			gameObjects[i]->Update();
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