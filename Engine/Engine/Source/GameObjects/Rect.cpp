#include "stdafx.h"
#include "GameObjects\Rect.h"
#include "Renderer\Renderer.h"
#include "Managers\Physics.h"
#include "XEngine.h"

Rect::Rect(Vector2 spawn_pos, XEngine& ref, int _width, int _height, D2D1::ColorF _color) : GameObject(spawn_pos, ref) {
	width = _width;
	height = _height;
	color = _color;
}

void Rect::OnRender(Renderer &renderer){
	GameObject::OnRender(renderer);
	Transform worldPos;
	WorldTransform(&worldPos);
	SetTransform(renderer, width, height);
	renderer.RenderRect(worldPos.position.x, worldPos.position.y, width, height, color, scale, fill, strokeWith);
}

void Rect::Update(float deltaTime)
{
	GameObject::Update(deltaTime);
}

void Rect::SetPhysics(bool active, PhysicBodyType bodyType, float32 friction, bool isSensor)
{
	if (active && rigidBody == NULL) {
		InitializeRectPhysics(bodyType, friction, isSensor);
	}
	else if(rigidBody != NULL) {
		DestroyBody();
	}
}

void Rect::InitializeRectPhysics(PhysicBodyType bodyType, float32 friction, bool isSensor)
{
	anchor.Set(width / 2.f, height / 2.f);
	Vector2 bounds = Vector2(width / 2.f, height / 2.f);

	rigidBody = coreRef.physics->CreateBoxBody(transform->position, bounds, 1.0, friction, bodyType, isSensor);
	
}