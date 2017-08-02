#include "stdafx.h"
#include "GameObjects\Sprite.h"
#include "Renderer\Renderer.h"
#include "XEngine.h"

Sprite::Sprite(b2Vec2 spawn_position, XEngine& ref, CachedImage &image) : GameObject(spawn_position, ref)
{
	cachedImage = image;
	frameWidth = image.Get2D2Bitmap()->GetPixelSize().width;
	frameHeight = image.Get2D2Bitmap()->GetPixelSize().height;
	columns = 1;
	rows = 1;
	animationManager.parent = this;
	//transform.q.Set(DEGREES_TO_RADS(180.f));
	scale.width = 1;
	scale.height = -1;
}

Sprite::~Sprite() {

}

void Sprite::OnRender(Renderer &renderer)
{
	GameObject::OnRender(renderer);
	b2Transform worldPos;
	WorldTransform(worldPos);
	int column = columns;

	if (column > columns - 1) {
		column = currentFrame % columns;
	}
	int row = (int) floor(currentFrame / columns);
	SetTransform(renderer, frameWidth, frameHeight);

	renderer.RenderImage(worldPos.p.x, worldPos.p.y, cachedImage, column, row, currentFrame, frameWidth, frameHeight, scale);
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

void Sprite::SetSpriteSheet(int newFrameWidth, int newFrameHeight)
{
	columns = (frameWidth / newFrameWidth);
	rows = (frameHeight / newFrameHeight);

	frameWidth = newFrameWidth;
	frameHeight = newFrameHeight;
}

void Sprite::InitializeSpritePhysics(PhysicShape shape, PhysicBodyType bodyType, float32 friction, bool isSensor, float32 radius)
{
	anchor.Set(frameWidth / 2.f, frameHeight / 2.f);

	switch (shape)
	{
	case PhysicShape::Circle:
	{
		rigidBody = coreRef.physics->CreateCircleBody(transform.p, radius, 1.0, friction, bodyType, isSensor);
		break;
	}
	case PhysicShape::Box:
	{
		b2Vec2 bounds = b2Vec2(frameWidth / 2.f, frameHeight / 2.f);
		rigidBody = coreRef.physics->CreateBoxBody(transform.p, bounds, 1.0, friction, bodyType, isSensor);
		break;
	}
	default:
		break;
	}	
	float32 angle = transform.q.GetAngle();
	rigidBody->SetTransform(rigidBody->GetPosition(), angle);
}

void Sprite::Update(float deltaTime) {
	animationManager.Update(deltaTime);
}