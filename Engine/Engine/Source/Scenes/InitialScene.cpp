#include "Scenes\InitialScene.h"
#include "XEngine.h"
#include "Managers\AnimationManager.h"
#include "Utils\Animation.h"
#include "Managers\InputManager.h"
#include "GameObjects\Sprite.h"
#include "GameObjects\Rect.h"
#include "GameObjects\Circle.h"

Sprite* sprite;

void InitialScene::Start()
{
	CachedImage* image = CacheManager::GetInstance()->AddImage(L"Resources/Mario-Idle-Walk.png");
	sprite = new Sprite(b2Vec2(0.f, 0.f), *image);
	sprite->SetSpriteSheet(17, 33);
	sprite->scale.width = 2.0f;
	sprite->scale.height = 2.0f;
	int *idleFrames = new int[1]{ 0 };
	int *walkFrames = new int[4]{ 1, 2, 3, 4 };
	Animation* idle = new Animation(idleFrames, 900, 1);
	Animation* walk = new Animation(walkFrames, 150, 4);
	idle->loop = true;
	walk->loop = true;
	sprite->animationManager.AddAnim(L"Idle", idle);
	sprite->animationManager.AddAnim(L"Walk", walk);
	gameObjects.insert(sprite);
	sprite->animationManager.PlayAnim(L"Idle");

	DEFINE_DELEGATE(newDel, void(unsigned int)) = CREATE_MULTICAST_DELEGATE(XEngine::instance->inputManager->OnMouseDown, EngineScene, &EngineScene::Test, this);
	XEngine::instance->inputManager->OnMouseDown += newDel;
}

void InitialScene::Update(float deltaTime)
{
	EngineScene::Update(deltaTime);
	if (XEngine::instance->inputManager->IsDown(VK_RIGHT)) {
		if (!sprite->animationManager.IsPlaying(L"Walk")) {
			sprite->animationManager.PlayAnim(L"Walk");
		}
		sprite->scale.width = 2.0f;
		sprite->scale.height = 2.0f;
		sprite->transform.p.x += 60 * deltaTime;
	}
	else if (XEngine::instance->inputManager->IsDown(VK_LEFT) ){
		if (!sprite->animationManager.IsPlaying(L"Walk")) {
			sprite->animationManager.PlayAnim(L"Walk");
		}
		sprite->scale.width = -2.0f;
		sprite->transform.p.x -= 60 * deltaTime;
	}
	else {
		if (!sprite->animationManager.IsPlaying(L"Idle")) {
			sprite->animationManager.PlayAnim(L"Idle");
		}
	}
}
