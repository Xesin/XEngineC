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


Sprite* sprite;
Rect* rect;

InitialScene::InitialScene(XEngine& ref) : EngineScene(ref) {
}

Sprite* Mario;

DEFINE_DELEGATE(newDel, void(unsigned int));
void InitialScene::OnKeyDown(unsigned int keyCode)
{
	unsigned int test = VK_SPACE;
	if (keyCode == VK_SPACE) {
 		Mario->rigidBody->ApplyForce(b2Vec2(0.0f, 150.f), Mario->rigidBody->GetWorldCenter(), true);
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
	Mario = new Sprite(Vector2(50, 200.f), coreRef, *image);
	Mario->SetSpriteSheet(17, 33);

	//SET MARIO ANIM
	int idle[1] = { 0 };
	int walk[4] = { 1, 2, 3, 4 };
	Mario->animationManager.AddAnim(TEXT("idle"), new Animation(idle, 150, 1, true));
	Mario->animationManager.AddAnim(TEXT("walk"), new Animation(walk, 150, 4, true));
	
	//SET MARIO PHYSICS
	Mario->SetPhysics(true, PhysicShape::Circle, PhysicBodyType::Kinematic, false, 0.0f, 8.f);
	Mario->anchor.y = 26.f;
	b2MassData massData;
	Mario->rigidBody->GetMassData(&massData);
	massData.center = Renderer::PixelsToWorldUnits(Mario->anchor);
	Mario->rigidBody->SetMassData(&massData);
	Mario->rigidBody->SetFixedRotation(true);
	coreRef.physics->AddBoxFixture(Mario->rigidBody, Vector2(0.f, Mario->anchor.y - (Mario->frameHeight / 2.f)), Vector2(Mario->frameWidth / 2.f, Mario->frameHeight / 2.f), 0.0f, 0.0f, true);

	//OTHER PHYSICS OBJECTS (no need to add this objects to update list or render list, there are only present in the physics engine)
	coreRef.physics->CreateBoxBody(Vector2(40.f, 720.f / 2.f), Vector2(30.f, 720.f), 0.0f, 1.0f, PhysicBodyType::Static, false);
	coreRef.physics->CreateBoxBody(Vector2(1230.f, 720.f / 2.f), Vector2(30.f, 720.f), 0.0f, 1.0f, PhysicBodyType::Static, false);

	Rect* rect = new Rect(Vector2(1280.f / 2.f, 50.f), coreRef, 1280, 100, D2D1::ColorF(0.5f, 0.f, 0.5f));
	rect->SetPhysics(true, PhysicBodyType::Kinematic);

	for (int i = 0; i < 40; i++) {
		Circle* rectDyn = new Circle(Vector2(i * 25.f + 100.f, 400.f), coreRef, 15.f, 15.f, D2D1::ColorF(MathUtils::RandomInRange(0.f, 1.0f), MathUtils::RandomInRange(0.f, 1.0f), MathUtils::RandomInRange(0.f, 1.0f)));
		rectDyn->SetPhysics(true, PhysicBodyType::Dynamic);
		AddGameObject(rectDyn);

		rectDyn = new Circle(Vector2(i * 25.f + 110.f, 460.f), coreRef, 15.f, 15.f, D2D1::ColorF(MathUtils::RandomInRange(0.f, 1.0f), MathUtils::RandomInRange(0.f, 1.0f), MathUtils::RandomInRange(0.f, 1.0f)));
		rectDyn->SetPhysics(true, PhysicBodyType::Dynamic);
		AddGameObject(rectDyn);

		rectDyn = new Circle(Vector2(i * 25.f + 100.f, 490.f), coreRef, 15.f, 15.f, D2D1::ColorF(MathUtils::RandomInRange(0.f, 1.0f), MathUtils::RandomInRange(0.f, 1.0f), MathUtils::RandomInRange(0.f, 1.0f)));
		rectDyn->SetPhysics(true, PhysicBodyType::Dynamic);
		AddGameObject(rectDyn);

		rectDyn = new Circle(Vector2(i * 25.f + 110.f, 520.f), coreRef, 15.f, 15.f, D2D1::ColorF(MathUtils::RandomInRange(0.f, 1.0f), MathUtils::RandomInRange(0.f, 1.0f), MathUtils::RandomInRange(0.f, 1.0f)));
		rectDyn->SetPhysics(true, PhysicBodyType::Dynamic);
		AddGameObject(rectDyn);

		rectDyn = new Circle(Vector2(i * 25.f + 100.f, 550.f), coreRef, 15.f, 15.f, D2D1::ColorF(MathUtils::RandomInRange(0.f, 1.0f), MathUtils::RandomInRange(0.f, 1.0f), MathUtils::RandomInRange(0.f, 1.0f)));
		rectDyn->SetPhysics(true, PhysicBodyType::Dynamic);
		AddGameObject(rectDyn);
	}
	AddGameObject(rect);
	AddGameObject(Mario);
}

void InitialScene::OnDestroy() {
	coreRef.inputManager->OnKeyDown -= newDel;
}

void InitialScene::Update(float deltaTime)
{
	EngineScene::Update(deltaTime);
	b2Vec2 currentVel = Mario->rigidBody->GetLinearVelocity();
	if (coreRef.inputManager->IsDown(VK_RIGHT)) {
		if (!Mario->animationManager.IsPlaying(TEXT("walk"))) {
			Mario->animationManager.PlayAnim(TEXT("walk"));
		}

		Mario->rigidBody->SetLinearVelocity(b2Vec2(5.f, currentVel.y));

		Mario->scale.width = 1;
	}else if (coreRef.inputManager->IsDown(VK_LEFT)) {
		if (!Mario->animationManager.IsPlaying(TEXT("walk"))) {
			Mario->animationManager.PlayAnim(TEXT("walk"));
		}
		Mario->rigidBody->SetLinearVelocity(b2Vec2(-5.f, currentVel.y));

		Mario->scale.width = -1;
	}
	else {
		if (!Mario->animationManager.IsPlaying(TEXT("idle"))) {
			Mario->animationManager.PlayAnim(TEXT("idle"));
		}
		Mario->rigidBody->SetLinearVelocity(b2Vec2(0.f, currentVel.y));
	}

	currentVel = Mario->rigidBody->GetLinearVelocity();

	if (coreRef.inputManager->IsDown(VK_UP)) {
		Mario->rigidBody->SetLinearVelocity(b2Vec2(currentVel.x, 5.f));
		b2Vec2 vel = Mario->rigidBody->GetLinearVelocity();
		if (vel.x > 5.f) {
			vel.x = 5.f;
			Mario->rigidBody->SetLinearVelocity(vel);
		}
	}
	else if (coreRef.inputManager->IsDown(VK_DOWN)) {
		Mario->rigidBody->SetLinearVelocity(b2Vec2(currentVel.x, -5.f));
		b2Vec2 vel = Mario->rigidBody->GetLinearVelocity();
		if (vel.x < -5.f) {
			vel.x = -5.f;
			Mario->rigidBody->SetLinearVelocity(vel);
		}

	}
	else {
		Mario->rigidBody->SetLinearVelocity(b2Vec2(currentVel.x, 0.f));
	}

	if (coreRef.inputManager->IsDown(VK_L)) {
		coreRef.camera->position.y += 20.f * deltaTime;
	}
	
}
