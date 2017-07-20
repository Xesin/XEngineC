#pragma once
#include "Utils\ArrayList.h"
#include "GameObjects\Sprite.h"
#include "GameObjects\GameObject.h"
#include "Renderer\Renderer.h"
#include "Managers\CacheManager.h"
#include "Managers\AnimationManager.h"
#include "Utils\Animation.h"
#include "Managers\InputManager.h"

class EngineScene {

public:
	~EngineScene() {
		gameObjects.empty();
	}

	void Preload() {}

	void Test(unsigned int keycode) {
		gameObjects[0]->transform.p.y += 10 ;
	}

	void Start() {
		Sprite* sprite = new Sprite(b2Vec2(50.f, 50.f), CacheManager::GetInstance()->AddImage(L"Resources/count anim.png"));
		sprite->SetSpriteSheet(50, 50);
		sprite->currentFrame = 0;
		int animFrames[] = { 1, 2, 3, 4 };
		Animation* anim = new Animation(animFrames, 200);
		anim->loop = true;
		sprite->animationManager->AddAnim(L"Idle", anim);
		gameObjects.insert(sprite);
		sprite->animationManager->PlayAnim(L"Idle");

		delegate<void(unsigned int)> newDel = CREATE_MULTICAST_DELEGATE(XEngine::instance->inputManager->OnMouseDown, EngineScene, &EngineScene::Test, this);
		XEngine::instance->inputManager->OnMouseDown += newDel;
	}
	void Update(float deltaTime) {
		for (int i = 0; i < gameObjects.size_of_list(); i++) {
			gameObjects[i]->Update(deltaTime);
		}

		if (XEngine::instance->inputManager->IsDown(VK_D)) {
			gameObjects[0]->transform.p.x += 60 * deltaTime;
		}
		if (XEngine::instance->inputManager->IsDown(VK_A)) {
			gameObjects[0]->transform.p.x -= 60 * deltaTime;
		}
		if (XEngine::instance->inputManager->IsDown(VK_S)) {
			gameObjects[0]->transform.p.y += 60 * deltaTime;
		}
		if (XEngine::instance->inputManager->IsDown(VK_W)) {
			gameObjects[0]->transform.p.y -= 60 * deltaTime;
			//POINT point = XEngine::instance->inputManager->CursorPos();
			gameObjects[0]->transform.p.y += 60;
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