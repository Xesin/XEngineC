#include "stdafx.h"
#include "GameObjects\Rect.h"
#include "Renderer\Renderer.h"
#include "Managers\Physics.h"
#include "XEngine.h"

Rect::Rect(b2Vec2 spawn_pos, XEngine& ref, int _width, int _height, D2D1::ColorF _color) : GameObject(spawn_pos, ref) {
	width = _width;
	height = _height;
	color = _color;
}

void Rect::SetPhysics(bool active, bool dynamic, float32 friction)
{
	if (active && rigidBody == NULL) {
		InitializeRectPhysics(dynamic, friction);
	}
	else if(rigidBody != NULL) {
		DestroyBody();
	}
}

void Rect::InitializeRectPhysics(bool dynamic, float32 friction)
{
	anchor.Set(0.5f, 0.5f);
	b2BodyDef bodyDef;
	b2Vec2 worldPos = Renderer::ScreenToWorldUnits(transform.p);
	bodyDef.position.Set(worldPos.x, worldPos.y);
	b2PolygonShape box;
	b2Vec2 worldBounds = Renderer::ScreenToWorldUnits(b2Vec2(width / 2.f, height / 2.f));
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
}

void Rect::OnRender(Renderer &renderer){
	b2Transform worldPos;
	WorldTransform(worldPos);
	SetTransform(renderer, width, height);
	renderer.RenderRect(worldPos.p.x, worldPos.p.y, width, height, color, scale, fill, strokeWith);
}

void Rect::Update(float deltaTime)
{
	if (rigidBody != NULL) {
		transform.p = Renderer::WorldToScreenPixels(rigidBody->GetPosition());
		transform.q.Set(rigidBody->GetAngle());
	}
}
