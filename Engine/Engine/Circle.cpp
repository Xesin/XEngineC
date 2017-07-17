#include "stdafx.h"
#include "Circle.h"
#include "Renderer.h"

Circle::Circle(b2Vec2 spawn_pos, float32 _radiusX, float32 _radiusY) : GameObject(spawn_pos) {
	radiusX = _radiusX;
	radiusY = _radiusY;

}

void Circle::OnRender(Renderer* renderer) const {
	renderer->RenderCircle(transform.p.x, transform.p.y, radiusX, radiusY, D2D1::ColorF(D2D1::ColorF::Aquamarine), fill, strokeWith);
}
