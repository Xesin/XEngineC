#include "stdafx.h"
#include "GameObjects\Rect.h"
#include "Renderer\Renderer.h"
#include "Managers\Physics.h"
#include "XEngine.h"

Rect::Rect(Vector2 spawn_pos, XEngine& ref, int width, int height, D2D1::ColorF _color) : GameObject(spawn_pos, ref) {
	bounds.x = (float) width;
	bounds.y = (float) height;
	color = _color;
}

void Rect::OnRender(Renderer &renderer){
	Transform worldPos;
	WorldTransform(&worldPos);
	renderer.SetTransform(worldPos.position, Vector2(bounds.x, bounds.y), transform->scale, anchor, transform->rotation.angles);

	renderer.RenderRect(worldPos.position.x, worldPos.position.y, (float) bounds.x, (float) bounds.y, color, transform->scale, fill, strokeWith);
	GameObject::OnRender(renderer);
}

void Rect::Update(float deltaTime)
{
	GameObject::Update(deltaTime);
}