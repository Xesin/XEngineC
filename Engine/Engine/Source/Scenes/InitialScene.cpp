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

Sprite* sprite;
Rect* rect;
GameObject* tilledImage;
InitialScene::InitialScene(XEngine& ref) : EngineScene(ref) {
}

Sprite* Mario;

DEFINE_DELEGATE(newDel, void(unsigned int));
void InitialScene::OnKeyDown(unsigned int keyCode)
{
	unsigned int test = VK_SPACE;
	if (keyCode == VK_SPACE) {
 		Mario->rigidBody->ApplyForce(Vector2(0.0f, 150.f), Mario->rigidBody->GetWorldCenter());
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
	newDel = CREATE_MULTICAST_DELEGATE(coreRef.inputManager->OnMouseDown, InitialScene, &InitialScene::OnKeyDown, this);
	coreRef.inputManager->OnKeyDown += newDel;
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

	tilledImage = new GameObject(Vector2(10.f, 10.f), coreRef);
	TilledImageRenderer* tilledRenderer = tilledImage->AddComponent<TilledImageRenderer>(false, true);

	pugi::xml_document doc;

	pugi::xml_parse_result result = doc.load_file("Resources/tree.xml");

	const char* resultText = result.description();
	pugi::xml_node node = doc.child("test").child("child1").first_child();
	tilledRenderer->SetImage(image);
	tilledRenderer->SetSpriteSheet(17, 33);
	tilledRenderer->SetTilleSize(Vector2(200.f, 200.f));
	//OTHER PHYSICS OBJECTS (no need to add this objects to update list or render list, there are only present in the physics engine)
	/*coreRef.physics->CreateBoxBody(Vector2(40.f, 720.f / 2.f), Vector2(30.f, 720.f), 0.0f, 1.0f, PhysicBodyType::Static, false);
	coreRef.physics->CreateBoxBody(Vector2(1230.f, 720.f / 2.f), Vector2(30.f, 720.f), 0.0f, 1.0f, PhysicBodyType::Static, false);*/

	Rect* rect = new Rect(Vector2(1280.f / 2.f, 50.f), coreRef, 1280, 100, D2D1::ColorF(0.5f, 0.f, 0.5f));
	/*rect->AddComponent<PhysicsBody>();
	rect->rigidBody->SetType(PhysicBodyType::Static);
	rect->rigidBody->AddSquareShape(Vector2(0.f, 0.f), Vector2(rect->bounds.x / 2.f, rect->bounds.y / 2.f), false, 1.0);*/
	for (int i = 0; i < 40; i++) {
		/*Circle* rectDyn = new Circle(Vector2(i * 25.f + 100.f, 400.f), coreRef, 15.f, 15.f, D2D1::ColorF(MathUtils::RandomInRange(0.f, 1.0f), MathUtils::RandomInRange(0.f, 1.0f), MathUtils::RandomInRange(0.f, 1.0f)));
		rectDyn->AddComponent<PhysicsBody>();
		rectDyn->rigidBody->SetType(PhysicBodyType::Dynamic);
		rectDyn->rigidBody->AddCircleShape(Vector2(0.f, 0.f), 15.f, false, 1.f);
		AddGameObject(rectDyn);

		rectDyn = new Circle(Vector2(i * 25.f + 110.f, 460.f), coreRef, 15.f, 15.f, D2D1::ColorF(MathUtils::RandomInRange(0.f, 1.0f), MathUtils::RandomInRange(0.f, 1.0f), MathUtils::RandomInRange(0.f, 1.0f)));
		rectDyn->AddComponent<PhysicsBody>();
		rectDyn->rigidBody->SetType(PhysicBodyType::Dynamic);
		rectDyn->rigidBody->AddCircleShape(Vector2(0.f, 0.f), 15.f, false, 1.f);
		AddGameObject(rectDyn);

		rectDyn = new Circle(Vector2(i * 25.f + 100.f, 490.f), coreRef, 15.f, 15.f, D2D1::ColorF(MathUtils::RandomInRange(0.f, 1.0f), MathUtils::RandomInRange(0.f, 1.0f), MathUtils::RandomInRange(0.f, 1.0f)));
		rectDyn->AddComponent<PhysicsBody>();
		rectDyn->rigidBody->SetType(PhysicBodyType::Dynamic);
		rectDyn->rigidBody->AddCircleShape(Vector2(0.f, 0.f), 15.f, false, 1.f);
		AddGameObject(rectDyn);

		rectDyn = new Circle(Vector2(i * 25.f + 110.f, 520.f), coreRef, 15.f, 15.f, D2D1::ColorF(MathUtils::RandomInRange(0.f, 1.0f), MathUtils::RandomInRange(0.f, 1.0f), MathUtils::RandomInRange(0.f, 1.0f)));
		rectDyn->AddComponent<PhysicsBody>();
		rectDyn->rigidBody->SetType(PhysicBodyType::Dynamic);
		rectDyn->rigidBody->AddCircleShape(Vector2(0.f, 0.f), 15.f, false, 1.f);
		AddGameObject(rectDyn);

		rectDyn = new Circle(Vector2(i * 25.f + 100.f, 550.f), coreRef, 15.f, 15.f, D2D1::ColorF(MathUtils::RandomInRange(0.f, 1.0f), MathUtils::RandomInRange(0.f, 1.0f), MathUtils::RandomInRange(0.f, 1.0f)));
		rectDyn->AddComponent<PhysicsBody>();
		rectDyn->rigidBody->SetType(PhysicBodyType::Dynamic);
		rectDyn->rigidBody->AddCircleShape(Vector2(0.f, 0.f), 15.f, false, 1.f);
		AddGameObject(rectDyn);*/

		/*rectDyn = new Circle(Vector2(i * 25.f + 100.f, 580.f), coreRef, 15.f, 15.f, D2D1::ColorF(MathUtils::RandomInRange(0.f, 1.0f), MathUtils::RandomInRange(0.f, 1.0f), MathUtils::RandomInRange(0.f, 1.0f)));
		rectDyn->AddComponent<PhysicsBody>();
		rectDyn->rigidBody->SetType(PhysicBodyType::Dynamic);
		rectDyn->rigidBody->AddCircleShape(Vector2(0.f, 0.f), 15.f, false, 1.f);
		AddGameObject(rectDyn);

		rectDyn = new Circle(Vector2(i * 25.f + 100.f, 610.f), coreRef, 15.f, 15.f, D2D1::ColorF(MathUtils::RandomInRange(0.f, 1.0f), MathUtils::RandomInRange(0.f, 1.0f), MathUtils::RandomInRange(0.f, 1.0f)));
		rectDyn->AddComponent<PhysicsBody>();
		rectDyn->rigidBody->SetType(PhysicBodyType::Dynamic);
		rectDyn->rigidBody->AddCircleShape(Vector2(0.f, 0.f), 15.f, false, 1.f);
		AddGameObject(rectDyn);*/
	}
	//AddGameObject(rect);
	//AddGameObject(tilledImage);
	//AddGameObject(Mario);
	
	AddTiledMap("Resources/TestTMX.tmx", Mario, 2);
}

void InitialScene::OnDestroy() {
	coreRef.inputManager->OnKeyDown -= newDel;
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
		tilledImage->GetTransform().position.x += 60.f *deltaTime;
	}else if (coreRef.inputManager->IsDown(VK_LEFT)) {
		if (!Mario->animationManager.IsPlaying(TEXT("walk"))) {
			Mario->animationManager.PlayAnim(TEXT("walk"));
		}
		Mario->rigidBody->SetLinearVelocity(Vector2(-5.f, currentVel.y));

		Mario->GetTransform().scale.x = -1;
		tilledImage->GetTransform().position.x -= 60.f *deltaTime;
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
		tilledImage->GetTransform().position.y += 60.f *deltaTime;
	}
	else if (coreRef.inputManager->IsDown(VK_DOWN)) {
		Mario->rigidBody->SetLinearVelocity(Vector2(currentVel.x, -5.f));
		Vector2 vel = Mario->rigidBody->GetLinearVelocity();
		if (vel.x < -5.f) {
			vel.x = -5.f;
			Mario->rigidBody->SetLinearVelocity(vel);
		}
		tilledImage->GetTransform().position.y -= 60.f *deltaTime;

	}
	else {
		Mario->rigidBody->SetLinearVelocity(Vector2(currentVel.x, 0.f));
	}

	if (coreRef.inputManager->IsDown(VK_L)) {
		coreRef.camera->position.y += 20.f * deltaTime;
	}
	
}
