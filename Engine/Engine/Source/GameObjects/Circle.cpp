#include "stdafx.h"
#include "GameObjects\Circle.h"
#include "Renderer\Renderer.h"
#include "XEngine.h"

Circle::Circle(b2Vec2 spawn_pos, XEngine& ref, float32 _radiusX, float32 _radiusY, D2D1::ColorF newColor) : GameObject(spawn_pos, ref) {
	radiusX = _radiusX;
	radiusY = _radiusY;
	color = newColor;
}

void Circle::SetPhysics(bool active, bool dynamic, float32 friction)
{
}

void Circle::OnRender(Renderer &renderer) {
	SetTransform(renderer, radiusX * 2.f, radiusY * 2.f);
	renderer.RenderCircle(transform.p.x, transform.p.y, radiusX, radiusY, color, scale, fill, strokeWith);
}
