#include "stdafx.h"
#include "GameObjects\Sprite.h"
#include "Renderer\Renderer.h"
#include "Component\SpriteRenderer.h"
#include "XEngine.h"

Sprite::Sprite(Vector2 spawn_position, XEngine& ref, CachedImage &image) : GameObject(spawn_position, ref)
{
	spriteRenderer = AddComponent<SpriteRenderer>(false, true);
	spriteRenderer->SetImage(&image);

	cachedImage = image;
	frameWidth = image.Get2D2Bitmap()->GetPixelSize().width;
	frameHeight = image.Get2D2Bitmap()->GetPixelSize().height;
	columns = 1;
	rows = 1;
	animationManager.parent = this;
}

Sprite::~Sprite() {

}

void Sprite::Update(float deltaTime) {
	GameObject::Update(deltaTime);
	animationManager.Update(deltaTime);
}

void Sprite::OnRender(Renderer &renderer)
{
	spriteRenderer->anchor.x = anchor.x;
	spriteRenderer->anchor.y = anchor.y;

	GameObject::OnRender(renderer);
}

void Sprite::SetPhysics(bool active, PhysicShape shape, PhysicBodyType bodyType, bool isSensor, float32 friction, float32 radius)
{
	if (active && rigidBody == NULL) {
		InitializeSpritePhysics(shape, bodyType, friction, isSensor, radius);
	}
	else if (rigidBody != NULL) {
		DestroyBody();
	}
}

void Sprite::InitializeSpritePhysics(PhysicShape shape, PhysicBodyType bodyType, float32 friction, bool isSensor, float32 radius)
{
	Vector2 bounds = Vector2(spriteRenderer->frameWidth / 2.f, spriteRenderer->frameHeight / 2.f);
	anchor.Set(bounds.x, bounds.y);

	switch (shape)
	{
	case PhysicShape::Circle:
	{
		rigidBody = coreRef.physics->CreateCircleBody(transform->position, radius, 1.0, friction, bodyType, isSensor);
		break;
	}
	case PhysicShape::Box:
	{
		rigidBody = coreRef.physics->CreateBoxBody(transform->position, bounds, 1.0, friction, bodyType, isSensor);
		break;
	}
	default:
		break;
	}	
	float32 angle = DEGREES_TO_RADS(transform->rotation.angles);
	rigidBody->SetTransform(rigidBody->GetPosition(), angle);
}

void Sprite::SetSpriteSheet(int newFrameWidth, int newFrameHeight)
{
	frameHeight = newFrameHeight;
	frameWidth = newFrameWidth;
	spriteRenderer->SetSpriteSheet(newFrameWidth, newFrameHeight);
}
