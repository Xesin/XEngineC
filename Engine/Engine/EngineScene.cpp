#include "stdafx.h"
#include "EngineScene.h"
#include "GameObject.h"
#include "Renderer.h"

EngineScene::~EngineScene()
{
	gameObjects.empty();
}

void EngineScene::Preload()
{
}

void EngineScene::Start()
{
}

void EngineScene::Update()
{
	
}

void EngineScene::OnDestroy()
{
}

void EngineScene::Render(HWND hwnd, Renderer* renderer)
{
	for (int i = 0; i < gameObjects.size_of_list(); i++) {
		renderer->OnRender(hwnd, gameObjects[i]);
	}
}
