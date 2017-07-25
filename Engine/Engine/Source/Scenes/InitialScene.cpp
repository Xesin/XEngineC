#include "Scenes\InitialScene.h"
#include "XEngine.h"
#include "Managers\AnimationManager.h"
#include "Utils\Animation.h"
#include "Managers\InputManager.h"
#include "GameObjects\Sprite.h"
#include "GameObjects\Rect.h"
#include "GameObjects\Circle.h"

Sprite* sprite;
Rect* rect;

DEFINE_DELEGATE(newDel, void(unsigned int));
void InitialScene::OnKeyDown(unsigned int keyCode)
{
	if (keyCode == VK_ESCAPE) {
		XEngine::instance->StartScene(new InitialScene());
	}
}


void InitialScene::Start()
{
	CachedImage* image = CacheManager::GetInstance()->AddImage(L"Resources/Mario-Idle-Walk.png");
	sprite = new Sprite(b2Vec2(0.f, 0.f), *image);
	sprite->SetSpriteSheet(17, 33);
	sprite->scale.width = 2.0f;
	sprite->scale.height = 2.0f;
	sprite->anchor.x = 0.5f;
	sprite->anchor.y = 0.5f;
	int *idleFrames = new int[1]{ 0 };
	int *walkFrames = new int[4]{ 1, 2, 3, 4 };
	Animation* idle = new Animation(idleFrames, 900, 1);
	Animation* walk = new Animation(walkFrames, 150, 4);
	idle->loop = true;
	walk->loop = true;
	sprite->animationManager.AddAnim(L"Idle", idle);
	sprite->animationManager.AddAnim(L"Walk", walk);
	sprite->animationManager.PlayAnim(L"Idle");
	gameObjects.insert(sprite);

	Rect* rect2 = new Rect(b2Vec2(0.f, 60.f), 50.f, 50.f, D2D1::ColorF(D2D1::ColorF::Red));
	rect = new Rect(b2Vec2(0.f, 0.f), 50.f, 50.f, D2D1::ColorF(D2D1::ColorF::Black));
	rect->anchor.x = 0.5f;
	rect->anchor.y = 0.5f;
	rect->scale.width = 3.f;
	rect->scale.height = 3.f;
	rect->transform.q.Set(DEGREES_TO_RADS(90.f));
	/*gameObjects.insert(rect);
	gameObjects.insert(rect2);*/
	newDel = CREATE_MULTICAST_DELEGATE(XEngine::instance->inputManager->OnMouseDown, InitialScene, &InitialScene::OnKeyDown, this);
	//XEngine::instance->inputManager->OnKeyDown += newDel;
}

void InitialScene::OnDestroy() {
	XEngine::instance->inputManager->OnKeyDown -= newDel;
}

void InitialScene::Update(float deltaTime)
{
	EngineScene::Update(deltaTime);
	if (XEngine::instance->inputManager->IsDown(VK_RIGHT)) {
		if (!sprite->animationManager.IsPlaying(L"Walk")) {
			sprite->animationManager.PlayAnim(L"Walk");
		}
		sprite->scale.width = 2.0f;
		sprite->transform.p.x += 60 * deltaTime;
		rect->transform.p.x += 60 * deltaTime;
	}
	else if (XEngine::instance->inputManager->IsDown(VK_LEFT) ){
		if (!sprite->animationManager.IsPlaying(L"Walk")) {
			sprite->animationManager.PlayAnim(L"Walk");
		}
		sprite->scale.width = -2.0f;
		sprite->transform.p.x -= 60 * deltaTime;
		rect->transform.p.x -= 60 * deltaTime;
	}
	else if (XEngine::instance->inputManager->IsDown(VK_UP)) {
		if (!sprite->animationManager.IsPlaying(L"Walk")) {
		sprite->animationManager.PlayAnim(L"Walk");
		}
		sprite->transform.p.y -= 60 * deltaTime;
		rect->transform.p.y -= 60 * deltaTime;
	}
	else if (XEngine::instance->inputManager->IsDown(VK_DOWN)) {
		if (!sprite->animationManager.IsPlaying(L"Walk")) {
		sprite->animationManager.PlayAnim(L"Walk");
		}
		sprite->transform.p.y += 60 * deltaTime;
		rect->transform.p.y += 60 * deltaTime;
	}
	else {
		if (!sprite->animationManager.IsPlaying(L"Idle")) {
			sprite->animationManager.PlayAnim(L"Idle");
		}
	}

	
}
