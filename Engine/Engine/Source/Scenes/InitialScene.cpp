#include "Scenes\InitialScene.h"
#include "XEngine.h"
#include "Managers\AnimationManager.h"
#include "Utils\Animation.h"
#include "Managers\InputManager.h"
#include "GameObjects\Sprite.h"
#include "GameObjects\Rect.h"
#include "GameObjects\Circle.h"

void InitialScene::Start()
{
	Sprite* sprite = new Sprite(b2Vec2(50.f, 50.f), CacheManager::GetInstance()->AddImage(L"Resources/count anim.png"));
	sprite->SetSpriteSheet(50, 50);
	sprite->scale.height = 0.5f;

	Rect* rect = new Rect(b2Vec2(50.f, 50.f), 100.f, 100.f, D2D1::ColorF(D2D1::ColorF::Black));
	Circle* circle = new Circle(b2Vec2(50.f, 50.f), 10.f, 10.f, D2D1::ColorF(D2D1::ColorF::Black));

	int animFrames[] = { 1, 2, 3, 4 };
	Animation* anim = new Animation(animFrames, 200);
	anim->loop = true;
	sprite->animationManager->AddAnim(L"Idle", anim);
	gameObjects.insert(sprite);
	gameObjects.insert(rect);
	gameObjects.insert(circle);
	sprite->animationManager->PlayAnim(L"Idle");

	delegate<void(unsigned int)> newDel = CREATE_MULTICAST_DELEGATE(XEngine::instance->inputManager->OnMouseDown, EngineScene, &EngineScene::Test, this);
	XEngine::instance->inputManager->OnMouseDown += newDel;
}

void InitialScene::Update(float deltaTime)
{
	EngineScene::Update(deltaTime);
	if (XEngine::instance->inputManager->IsMouseButtonDown(MOUSE_LEFT)) {

		POINT point = XEngine::instance->inputManager->CursorPos();
		gameObjects[0]->transform.p.x = point.x;
		gameObjects[0]->transform.p.y = point.y;
	}
}
