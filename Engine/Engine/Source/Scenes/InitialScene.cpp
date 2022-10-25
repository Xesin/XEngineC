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

DECLARE_DELEGATE(asdfasdf, unsigned int);

Sprite* sprite;
Rect* rect;
OnKeyDelegate_delegate onKeyDownDelegate;
InitialScene::InitialScene(XEngine& ref) : EngineScene(ref) {
}

Sprite* character;

void InitialScene::OnKeyDown(unsigned int keyCode)
{
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

	ADD_MULTICAST_DELEGATE(onKeyDownDelegate, coreRef.inputManager->OnMouseDown, InitialScene, &InitialScene::OnKeyDown, this);
	ADD_MULTICAST_DELEGATE(onKeyDownDelegate, coreRef.inputManager->OnKeyDown, InitialScene, &InitialScene::OnKeyDown, this);
	CachedImage* image = CacheManager::GetInstance()->AddImage(TEXT("Resources/character tiles.png"));
	character = new Sprite(Vector2(0.f, 0.f), coreRef, *image);
	character->SetSpriteSheet(32, 48);
	character->AddComponent<PhysicsBody>();
	character->rigidBody->SetType(PhysicBodyType::Dynamic);
	//SET MARIO ANIM
	int idle[1] = { 0 };
	int walk[4] = { 1, 2, 3 };
	character->animationManager.AddAnim(TEXT("idle"), new Animation(idle, 150, 1, true));
	character->animationManager.AddAnim(TEXT("walk"), new Animation(walk, 150, 4, true));
	
	//SET MARIO PHYSICS
	character->anchor.y = 26.f;
	b2MassData massData;
	character->rigidBody->GetMassData(&massData);
	character->rigidBody->SetGravityScale(0.f);
	Vector2 center = Renderer::PixelsToWorldUnits(character->anchor);

	massData.center = b2Vec2(center.x, center.y);
	character->rigidBody->SetMassData(&massData);
	character->rigidBody->SetFixedRotation(true);
	character->rigidBody->AddCircleShape(Vector2(0.f, 5.f), 6.f, false, 0.f);
	
	ADD_DELEGATE(OnTileReplace, InitialScene, &InitialScene::TileReplace, this);
	vector<int> tileIdToReplace = vector<int>();
	tileIdToReplace.push_back(81);
	
	wstring path = TEXT("Resources/TestTMX.tmx");
	AddTiledMap(path.c_str(), tileIdToReplace);
}

void InitialScene::OnDestroy() {
	coreRef.inputManager->OnKeyDown -= onKeyDownDelegate;
}

void InitialScene::Update(float deltaTime)
{
	EngineScene::Update(deltaTime);
	Vector2 currentVel = character->rigidBody->GetLinearVelocity();
	if (coreRef.inputManager->IsDown(VK_RIGHT)) {
		if (!character->animationManager.IsPlaying(TEXT("walk"))) {
			character->animationManager.PlayAnim(TEXT("walk"));
		}

		character->rigidBody->SetLinearVelocity(Vector2(50.f, currentVel.y));
		
		character->GetTransform().scale.x= 1;
	}else if (coreRef.inputManager->IsDown(VK_LEFT)) {
		if (!character->animationManager.IsPlaying(TEXT("walk"))) {
			character->animationManager.PlayAnim(TEXT("walk"));
		}
		character->rigidBody->SetLinearVelocity(Vector2(-50.f, currentVel.y));

		character->GetTransform().scale.x = -1;
	}
	else {
		if (!character->animationManager.IsPlaying(TEXT("idle"))) {
			character->animationManager.PlayAnim(TEXT("idle"));
		}
		character->rigidBody->SetLinearVelocity(Vector2(0.f, currentVel.y));
	}

	currentVel = character->rigidBody->GetLinearVelocity();

	if (coreRef.inputManager->IsDown(VK_UP)) {
		character->rigidBody->SetLinearVelocity(Vector2(currentVel.x, 50.f));
		Vector2 vel = character->rigidBody->GetLinearVelocity();
		if (vel.x > 50.f) {
			vel.x = 50.f;
			character->rigidBody->SetLinearVelocity(vel);
		}
	}
	else if (coreRef.inputManager->IsDown(VK_DOWN)) {
		character->rigidBody->SetLinearVelocity(Vector2(currentVel.x, -50.f));
		Vector2 vel = character->rigidBody->GetLinearVelocity();
		if (vel.x < -50.f) {
			vel.x = -50.f;
			character->rigidBody->SetLinearVelocity(vel);
		}

	}
	else {
		character->rigidBody->SetLinearVelocity(Vector2(currentVel.x, 0.f));
	}

	Transform& marioTransform = character->GetTransform();

	//coreRef.camera->position = marioTransform.position;

}

void InitialScene::TileReplace(unsigned int, Vector2 position)
{
	character->rigidBody->Translate(position);
	AddGameObject(character);
}
