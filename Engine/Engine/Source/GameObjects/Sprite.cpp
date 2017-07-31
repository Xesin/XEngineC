#include "stdafx.h"
#include "GameObjects\Sprite.h"
#include "Renderer\Renderer.h"
#include "XEngine.h"
#include "Managers\Physics.h"

Sprite::Sprite(b2Vec2 spawn_position, XEngine& ref, CachedImage &image) : GameObject(spawn_position, ref)
{
	cachedImage = image;
	frameWidth = image.Get2D2Bitmap()->GetPixelSize().width;
	frameHeight = image.Get2D2Bitmap()->GetPixelSize().height;
	columns = 1;
	rows = 1;
	animationManager.parent = this;
	transform.q.Set(DEGREES_TO_RADS(180.f));
	scale.width = -1;
}

Sprite::~Sprite() {

}

void Sprite::OnRender(Renderer &renderer)
{
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

void Sprite::SetPhysics(bool active, bool dynamic, float32 friction)
{
	if (active && rigidBody == NULL) {
		anchor.Set(0.5f, 0.5f);
		b2BodyDef bodyDef;
		b2Vec2 worldPos = coreRef.ScreenToWorldUnits(transform.p);
		bodyDef.position.Set(worldPos.x, worldPos.y);
		b2PolygonShape box;
		b2Vec2 worldBounds = coreRef.ScreenToWorldUnits(b2Vec2(frameWidth / 2.f, frameHeight / 2.f));
		box.SetAsBox(worldBounds.x, worldBounds.y);
		if (dynamic) {
			bodyDef.type = b2_dynamicBody;
			rigidBody = coreRef.physics->world.CreateBody(&bodyDef);
			b2FixtureDef fixtureDef;
			fixtureDef.shape = &box;
			fixtureDef.density = 1.0f;
			fixtureDef.friction = friction;
			rigidBody->CreateFixture(&fixtureDef);
		}
		else {
			rigidBody = coreRef.physics->world.CreateBody(&bodyDef);
			rigidBody->CreateFixture(&box, 0.0f);
		}
		float32 angle = transform.q.GetAngle();
		rigidBody->SetTransform(rigidBody->GetPosition(), angle);
	}
}

void Sprite::SetSpriteSheet(int newFrameWidth, int newFrameHeight)
{
	columns = (frameWidth / newFrameWidth);
	rows = (frameHeight / newFrameHeight);

	frameWidth = newFrameWidth;
	frameHeight = newFrameHeight;
}

void Sprite::Update(float deltaTime) {
	animationManager.Update(deltaTime);
	if (rigidBody != NULL) {
		transform.p = coreRef.WorldToScreenPixels(rigidBody->GetPosition());
		transform.q.Set(rigidBody->GetAngle());
	}
}