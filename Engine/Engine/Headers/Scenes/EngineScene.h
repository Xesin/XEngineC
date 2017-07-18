#pragma once
#include "Utils\ArrayList.h"
#include "GameObjects\Circle.h"
#include "GameObjects\Rect.h"
#include "GameObjects\GameObject.h"
#include "Renderer\Renderer.h"

class EngineScene {

public:
	~EngineScene() {
		gameObjects.empty();
	}

	void Preload() {}
	void Start() {
		Rect* rect = new Rect(b2Vec2(0.f, 0.f), 100, 100, D2D1::ColorF(1.0f, 0.2f, 0.2f, 1.0f));
		Rect* rect2 = new Rect(b2Vec2(255, 40.f), 100, 100, D2D1::ColorF(1.0f, 0.2f, 0.8f, 1.0f));
		Circle* ellipse = new Circle(b2Vec2(50.f, 40.f), 100, 100);
		ellipse->fill = false;
		ellipse->strokeWith = 5;
		rect->SetParent(ellipse);
		gameObjects.insert(rect);
		gameObjects.insert(ellipse);
		gameObjects.insert(rect2);
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