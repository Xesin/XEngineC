#pragma once
#include "ArrayList.h"

class GameObject;
class Renderer;

class EngineScene {

public:
	~EngineScene();

	void Preload();
	void Start();
	void Update();
	void OnDestroy();
	void Render(HWND hwnd, Renderer* renderer);

public:
	ArrayList<GameObject*> gameObjects;
};