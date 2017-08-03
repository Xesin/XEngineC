#include "stdafx.h"
#include "GameObjects\Sprite.h"
#include "Renderer\Renderer.h"
#include "XEngine.h"

Sprite::Sprite(Vector2 spawn_position, XEngine& ref, CachedImage &image) : GameObject(spawn_position, ref)
{
	cachedImage = image;
	frameWidth = image.Get2D2Bitmap()->GetPixelSize().width;
	frameHeight = image.Get2D2Bitmap()->GetPixelSize().height;
	columns = 1;
	rows = 1;
	animationManager.parent = this;
	scale.width = 1;
	scale.height = -1;
}

Sprite::~Sprite() {

}

void Sprite::Update(float deltaTime) {
	GameObject::Update(deltaTime);
	animationManager.Update(deltaTime);
}

void Sprite::OnRender(Renderer &renderer)
{
	Transform worldPos;
	WorldTransform(&worldPos);

	int column = currentFrame % columns;
	int row = (int) floor(((float)currentFrame / columns));

	SetTransform(renderer, frameWidth, frameHeight);

	renderer.RenderImage(worldPos.position.x, worldPos.position.y, cachedImage, column, row, currentFrame, frameWidth, frameHeight, scale);
	
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
	anchor.Set(frameWidth / 2.f, frameHeight / 2.f);

	switch (shape)
	{
	case PhysicShape::Circle:
	{
		rigidBody = coreRef.physics->CreateCircleBody(transform->position, radius, 1.0, friction, bodyType, isSensor);
		break;
	}
	case PhysicShape::Box:
	{
		Vector2 bounds = Vector2(frameWidth / 2.f, frameHeight / 2.f);
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
	columns = (frameWidth / newFrameWidth);
	rows = (frameHeight / newFrameHeight);

	frameWidth = newFrameWidth;
	frameHeight = newFrameHeight;
}
