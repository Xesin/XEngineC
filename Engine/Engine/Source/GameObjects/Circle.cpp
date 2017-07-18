#include "stdafx.h"
#include "GameObjects\Circle.h"
#include "Renderer\Renderer.h"
#include "Renderer\CachedImage.h"

CachedImage* cachedImage;

Circle::Circle(b2Vec2 spawn_pos, float32 _radiusX, float32 _radiusY) : GameObject(spawn_pos) {
	radiusX = _radiusX;
	radiusY = _radiusY;
	cachedImage = new CachedImage(L"Resources/Engine.ico");
}

void Circle::OnRender(Renderer* renderer) const {
	renderer->RenderCircle(transform.p.x, transform.p.y, radiusX, radiusY, D2D1::ColorF(D2D1::ColorF::Aquamarine), fill, strokeWith);
	renderer->RenderImage(50, 50, cachedImage);
}
