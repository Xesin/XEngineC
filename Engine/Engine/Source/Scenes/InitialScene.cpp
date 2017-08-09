#include "Scenes\InitialScene.h"
#include "XEngine.h"
#include "Managers\AnimationManager.h"
#include "Managers\InputManager.h"
#include "Utils\Animation.h"
#include "Utils\MathUtils.h"
#include "GameObjects\Sprite.h"
#include "GameObjects\Rect.h"
#include "GameObjects\Circle.h"
#include "Renderer\Camera.h"
#include "Component\TilledImageRenderer.h"
#include "Component\PhysicsBody.h"
#include "TiledImporter\TiledImporter.h"
#include "Managers\ScaleManager.h"

Sprite* sprite;
Rect* rect;
InitialScene::InitialScene(XEngine& ref) : EngineScene(ref) {
}

Sprite* Mario;

DEFINE_DELEGATE(onKeyDownDelegate, void(unsigned int));
void InitialScene::OnKeyDown(unsigned int keyCode)
{
	unsigned int test = VK_SPACE;
	if (keyCode == VK_SPACE) {
		coreRef.renderer->scaleManager->gameScale = Vector2(2.f, 2.f);
	}

	if (keyCode == VK_N) {
		coreRef.renderer->scaleManager->gameScale = Vector2(1.f, 1.f);
	}

	if (keyCode == VK_ESCAPE) {
		coreRef.StartScene(new InitialScene(coreRef));
	}

	if (keyCode == VK_D) {
		coreRef.physics->isDebug = !coreRef.physics->isDebug;
	}
}

void InitialScene::Start()
{
	coreRef.renderer->scaleManager->gameScale = Vector2(2.f, 2.f);
	coreRef.renderer->SetClearColor(D2D1::ColorF(0.16f, 0.15f, 0.20f));
	onKeyDownDelegate = CREATE_MULTICAST_DELEGATE(coreRef.inputManager->OnMouseDown, InitialScene, &InitialScene::OnKeyDown, this);
	coreRef.inputManager->OnKeyDown += onKeyDownDelegate;
	CachedImage* image = CacheManager::GetInstance()->AddImage(TEXT("Resources/Mario-Idle-Walk.png"));
	Mario = new Sprite(Vector2(100.f, 200.f), coreRef, *image);
	Mario->SetSpriteSheet(17, 33);
	Mario->AddComponent<PhysicsBody>();
	Mario->rigidBody->SetType(PhysicBodyType::Dynamic);
	//SET MARIO ANIM
	int idle[1] = { 0 };
	int walk[4] = { 1, 2, 3, 4 };
	Mario->animationManager.AddAnim(TEXT("idle"), new Animation(idle, 150, 1, true));
	Mario->animationManager.AddAnim(TEXT("walk"), new Animation(walk, 150, 4, true));
	
	//SET MARIO PHYSICS
	Mario->anchor.y = 26.f;
	b2MassData massData;
	Mario->rigidBody->GetMassData(&massData);
	Mario->rigidBody->SetGravityScale(0.f);
	Vector2 center = Renderer::PixelsToWorldUnits(Mario->anchor);

	massData.center = b2Vec2(center.x, center.y);
	Mario->rigidBody->SetMassData(&massData);
	Mario->rigidBody->SetFixedRotation(true);
	Mario->rigidBody->AddCircleShape(Vector2(0.f, 5.f), 6.f, false, 0.f);
	
	DEFINE_DELEGATE(replaceDel, void(unsigned int, Vector2));
	CREATE_DELEGATE(replaceDel, InitialScene, &InitialScene::TileReplace, this);
	AddTiledMap("Resources/TestTMX.tmx", replaceDel, {81});
}

void InitialScene::OnDestroy() {
	coreRef.inputManager->OnKeyDown -= onKeyDownDelegate;
}

void InitialScene::Update(float deltaTime)
{
	EngineScene::Update(deltaTime);
	Vector2 currentVel = Mario->rigidBody->GetLinearVelocity();
	if (coreRef.inputManager->IsDown(VK_RIGHT)) {
		if (!Mario->animationManager.IsPlaying(TEXT("walk"))) {
			Mario->animationManager.PlayAnim(TEXT("walk"));
		}

		Mario->rigidBody->SetLinearVelocity(Vector2(5.f, currentVel.y));
		
		Mario->GetTransform().scale.x= 1;
	}else if (coreRef.inputManager->IsDown(VK_LEFT)) {
		if (!Mario->animationManager.IsPlaying(TEXT("walk"))) {
			Mario->animationManager.PlayAnim(TEXT("walk"));
		}
		Mario->rigidBody->SetLinearVelocity(Vector2(-5.f, currentVel.y));

		Mario->GetTransform().scale.x = -1;
	}
	else {
		if (!Mario->animationManager.IsPlaying(TEXT("idle"))) {
			Mario->animationManager.PlayAnim(TEXT("idle"));
		}
		Mario->rigidBody->SetLinearVelocity(Vector2(0.f, currentVel.y));
	}

	currentVel = Mario->rigidBody->GetLinearVelocity();

	if (coreRef.inputManager->IsDown(VK_UP)) {
		Mario->rigidBody->SetLinearVelocity(Vector2(currentVel.x, 5.f));
		Vector2 vel = Mario->rigidBody->GetLinearVelocity();
		if (vel.x > 5.f) {
			vel.x = 5.f;
			Mario->rigidBody->SetLinearVelocity(vel);
		}
	}
	else if (coreRef.inputManager->IsDown(VK_DOWN)) {
		Mario->rigidBody->SetLinearVelocity(Vector2(currentVel.x, -5.f));
		Vector2 vel = Mario->rigidBody->GetLinearVelocity();
		if (vel.x < -5.f) {
			vel.x = -5.f;
			Mario->rigidBody->SetLinearVelocity(vel);
		}

	}
	else {
		Mario->rigidBody->SetLinearVelocity(Vector2(currentVel.x, 0.f));
	}

	coreRef.camera->position = Mario->GetTransform().position;
}

void InitialScene::TileReplace(unsigned int, Vector2 position)
{
	Mario->rigidBody->Translate(position);
	AddGameObject(Mario);
}
