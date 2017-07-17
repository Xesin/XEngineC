#pragma once
#include "ArrayList.h"
#include "Circle.h"
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
		Circle* ellipse = new Circle(b2Vec2(50.f, 40.f), 100, 100);
		ellipse->fill = false;
		ellipse->strokeWith = 5;
		gameObjects.insert(ellipse);
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