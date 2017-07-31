#include "stdafx.h"
#include "GameObjects\Rect.h"
#include "Renderer\Renderer.h"
#include "Managers\Physics.h"
#include "XEngine.h"

Rect::Rect(b2Vec2 spawn_pos, XEngine& ref, float32 _width, float32 _height, D2D1::ColorF _color) : GameObject(spawn_pos, ref) {
	width = _width;
	height = _height;
	color = _color;
}

void Rect::SetPhysics(bool active, bool dynamic)
{
	if (active && rigidBody == NULL) {
		b2BodyDef bodyDef;
		bodyDef.position.Set(transform.p.x, transform.p.y);
		b2PolygonShape box;
		box.SetAsBox(width / 2, height / 2);
		if (dynamic) {
			bodyDef.type = b2_dynamicBody;
			rigidBody = coreRef.physics->world.CreateBody(&bodyDef);
			b2FixtureDef fixtureDef;
			fixtureDef.shape = &box;
			fixtureDef.density = 1.0f;
			fixtureDef.friction = 0.3f;
			rigidBody->CreateFixture(&fixtureDef);
		}
		else {
			rigidBody = coreRef.physics->world.CreateBody(&bodyDef);
			rigidBody->CreateFixture(&box, 0.0f);
		}
	}
}

void Rect::OnRender(Renderer &renderer){
	b2Transform worldPos;
	WorldTransform(worldPos);
	SetRotation(renderer, width, height);
	renderer.RenderRect(worldPos.p.x, worldPos.p.y, width, height, color, scale, fill, strokeWith);
}

void Rect::Update(float deltaTime)
{
	if (rigidBody != NULL) {
		transform.p = rigidBody->GetPosition();
		transform.q.Set(rigidBody->GetAngle());
	}
}
