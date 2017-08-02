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
		coreRef.physics->isDebug = !coreRef.physics->isDebug;
	}
}

void InitialScene::Start()
{
	newDel = CREATE_MULTICAST_DELEGATE(coreRef.inputManager->OnMouseDown, InitialScene, &InitialScene::OnKeyDown, this);
	coreRef.inputManager->OnKeyDown += newDel;
	CachedImage* image = CacheManager::GetInstance()->AddImage(TEXT("Resources/Mario-Idle-Walk.png"));
	Mario = new Sprite(b2Vec2(50, 200.f), coreRef, *image);
	Mario->SetSpriteSheet(17, 33);
	int idle[1] = { 0 };
	int walk[4] = { 1, 2, 3, 4 };
	Mario->animationManager.AddAnim(TEXT("idle"), new Animation(idle, 100, 1));
	Mario->animationManager.AddAnim(TEXT("walk"), new Animation(idle, 100, 1));
	Mario->anchor.Set(0.5f, 0.5f);
	/*Mario->scale.height = -1.f;*/
	Mario->SetPhysics(true, PhysicShape::Box, PhysicBodyType::Kinematic, 0.0f, 10.f);
	Mario->rigidBody->SetFixedRotation(true);
	Mario->rigidBody->SetLinearDamping(5.f);

	Rect* rect = new Rect(b2Vec2(450.f, 50.f), coreRef, 900.f, 100.f, D2D1::ColorF(0.5f, 0.f, 0.5f));
	rect->SetPhysics(true, PhysicBodyType::Kinematic);
	rectDyn = new Rect(b2Vec2(120.f, 400.f), coreRef, 100.f, 100.f, D2D1::ColorF(0.5f, 0.f, 0.9f));
	rectDyn->SetPhysics(true, PhysicBodyType::Dynamic);
	
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

	if (coreRef.inputManager->IsDown(VK_RIGHT)) {
		if (Mario->animationManager.IsPlaying(TEXT("idle"))) {
			Mario->animationManager.PlayAnim(TEXT("walk"));
		}
		Mario->rigidBody->SetLinearVelocity(b2Vec2(5.f, 0.f));

		Mario->scale.width = -1;
	}else if (coreRef.inputManager->IsDown(VK_LEFT)) {
		if (Mario->animationManager.IsPlaying(TEXT("idle"))) {
			Mario->animationManager.PlayAnim(TEXT("walk"));
		}
		Mario->rigidBody->SetLinearVelocity(b2Vec2(-5.f, 0.f));

		Mario->scale.width = 1;
	}
	else {
		if (!Mario->animationManager.IsPlaying(TEXT("idle"))) {
			Mario->animationManager.PlayAnim(TEXT("idle"));
		}
		b2Vec2 vel = Mario->rigidBody->GetLinearVelocity();
		Mario->rigidBody->SetLinearVelocity(b2Vec2(0.f, vel.y));
	}

	if (coreRef.inputManager->IsDown(VK_UP)) {
		Mario->rigidBody->SetLinearVelocity(b2Vec2(0.f, 5.f));
		b2Vec2 vel = Mario->rigidBody->GetLinearVelocity();
		if (vel.x > 5.f) {
			vel.x = 5.f;
			Mario->rigidBody->SetLinearVelocity(vel);
		}
	}
	else if (coreRef.inputManager->IsDown(VK_DOWN)) {
		Mario->rigidBody->SetLinearVelocity(b2Vec2(0.f, -5.f));
		b2Vec2 vel = Mario->rigidBody->GetLinearVelocity();
		if (vel.x < -5.f) {
			vel.x = -5.f;
			Mario->rigidBody->SetLinearVelocity(vel);
		}

	}
	else {
		b2Vec2 vel = Mario->rigidBody->GetLinearVelocity();
		Mario->rigidBody->SetLinearVelocity(b2Vec2(vel.x, 0.f));
	}
	
}
