#include "Scenes\InitialScene.h"
#include "XEngine.h"
#include "Managers\AnimationManager.h"
#include "Managers\InputManager.h"
#include "Utils\Animation.h"
#include "GameObjects\Sprite.h"
#include "GameObjects\Rect.h"
#include "GameObjects\Circle.h"

Sprite* sprite;
Rect* rect;

InitialScene::InitialScene(XEngine& ref) : EngineScene(ref) {

}

Sprite* Mario;
Rect* rectDyn;
DEFINE_DELEGATE(newDel, void(unsigned int));
void InitialScene::OnKeyDown(unsigned int keyCode)
{
	unsigned int test = VK_SPACE;
	if (keyCode == VK_SPACE) {
 		Mario->rigidBody->ApplyForce(b2Vec2(0.0f, 150.f), Mario->rigidBody->GetWorldCenter(), true);
	}
	if (keyCode == VK_ESCAPE) {
		//Mario->Destroy();
		if (rectDyn->IsValid()) {
			rectDyn->Destroy();
		}
	}
	if (keyCode == VK_D) {
		coreRef.physics->drawDebug = !coreRef.physics->drawDebug;
	}
}

void InitialScene::Start()
{
	newDel = CREATE_MULTICAST_DELEGATE(coreRef.inputManager->OnMouseDown, InitialScene, &InitialScene::OnKeyDown, this);
	coreRef.inputManager->OnKeyDown += newDel;
	CachedImage* image = CacheManager::GetInstance()->AddImage(TEXT("Resources/Mario-Idle-Walk.png"));
	Mario = new Sprite(b2Vec2(50, 200.f), coreRef, *image);
	Mario->SetSpriteSheet(17, 33);
	//Mario->scale.height = -1.f;
	Mario->SetPhysics(true, PhysicShape::Box, true, 0.0f);
	Mario->rigidBody->SetFixedRotation(true);
	Mario->rigidBody->SetLinearDamping(5.f);
	Rect* rect = new Rect(b2Vec2(450.f, 50.f), coreRef, 900.f, 100.f, D2D1::ColorF(0.5f, 0.f, 0.5f));
	rect->SetPhysics(true, false);
	rectDyn = new Rect(b2Vec2(120.f, 400.f), coreRef, 100.f, 100.f, D2D1::ColorF(0.5f, 0.f, 0.9f));
	rectDyn->SetPhysics(true, true);
	gameObjects.insert(rect);
	gameObjects.insert(rectDyn);
	gameObjects.insert(Mario);
	updateList.insert(rect);
	updateList.insert(rectDyn);
	updateList.insert(Mario);
}

void InitialScene::OnDestroy() {
	coreRef.inputManager->OnKeyDown -= newDel;
}



void InitialScene::Update(float deltaTime)
{
	EngineScene::Update(deltaTime);

	/*if (coreRef.inputManager->IsDown(VK_RIGHT)) {
		Mario->rigidBody->ApplyForce(b2Vec2(10.f, 0.f), Mario->rigidBody->GetWorldCenter(), true);
		b2Vec2 vel = Mario->rigidBody->GetLinearVelocity();
		if (vel.x > 5.f) {
			vel.x = 5.f;
			Mario->rigidBody->SetLinearVelocity(vel);
		}
	}else if (coreRef.inputManager->IsDown(VK_LEFT)) {
		Mario->rigidBody->ApplyForce(b2Vec2(-10.f, 0.f), Mario->rigidBody->GetWorldCenter(), true);
		b2Vec2 vel = Mario->rigidBody->GetLinearVelocity();
		if (vel.x < -5.f) {
			vel.x = -5.f;
			Mario->rigidBody->SetLinearVelocity(vel);
		}

	}*/
	
}
