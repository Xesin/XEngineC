#include "stdafx.h"
#include "GameObjects\Circle.h"
#include "Renderer\Renderer.h"
#include "XEngine.h"

Circle::Circle(Vector2 spawn_pos, XEngine& ref, float32 _radiusX, float32 _radiusY, D2D1::ColorF newColor) : GameObject(spawn_pos, ref) {
	radiusX = _radiusX;
	radiusY = _radiusY;
	color = newColor;
}

void Circle::OnRender(Renderer &renderer) {
	SetTransform(renderer, radiusX * 2.f, radiusY * 2.f);
	renderer.RenderCircle(transform->position.x, transform->position.y, radiusX, radiusY, color, scale, fill, strokeWith);
	GameObject::OnRender(renderer);
}

void Circle::Update(float deltaTime)
{
	GameObject::Update(deltaTime);
}