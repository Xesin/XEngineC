#include "Scenes\InitialScene.h"
#include "XEngine.h"


void InitialScene::Start()
{
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

void InitialScene::Update(float deltaTime)
{
	EngineScene::Update(deltaTime);
	/*if (XEngine::instance->inputManager->IsDown(VK_D)) {
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
	POINT point = XEngine::instance->inputManager->CursorPos();
	gameObjects[0]->transform.p.y += 60;
	}*/

	if (XEngine::instance->inputManager->IsMouseButtonDown(MOUSE_LEFT)) {

		POINT point = XEngine::instance->inputManager->CursorPos();
		gameObjects[0]->transform.p.x = point.x;
		gameObjects[0]->transform.p.y = point.y;
	}
}
