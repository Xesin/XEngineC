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

InitialScene::InitialScene(XEngine& ref) : EngineScene(ref) {

}

DEFINE_DELEGATE(newDel, void(unsigned int));
void InitialScene::OnKeyDown(unsigned int keyCode)
{
	if (keyCode == VK_ESCAPE) {
		coreRef.StartScene(new InitialScene(coreRef));
	}
}


void InitialScene::Start()
{
	newDel = CREATE_MULTICAST_DELEGATE(coreRef.inputManager->OnMouseDown, InitialScene, &InitialScene::OnKeyDown, this);

	Rect* rect = new Rect(b2Vec2(0.f, 0.f), coreRef, 100.f, 100.f, D2D1::ColorF(0.5f, 0.f, 0.5f));
	rect->SetPhysics(true, false);
	Rect* rectDyn = new Rect(b2Vec2(0.f, 200.f), coreRef, 100.f, 100.f, D2D1::ColorF(0.5f, 0.f, 0.9f));
	rectDyn->SetPhysics(true, true);
	gameObjects.insert(rect);
	gameObjects.insert(rectDyn);
}

void InitialScene::OnDestroy() {
	coreRef.inputManager->OnKeyDown -= newDel;
}

void InitialScene::Update(float deltaTime)
{
	EngineScene::Update(deltaTime);
}
