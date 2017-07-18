#include "stdafx.h"
#include "GameObjects\Rect.h"
#include "Renderer\Renderer.h"

Rect::Rect(b2Vec2 spawn_pos, float32 _width, float32 _height) : GameObject(spawn_pos) {
	width = _width;
	height = _height;

}

void Rect::OnRender(Renderer* renderer) const{
	renderer->RenderRect(transform.p.x, transform.p.y, width, height, D2D1::ColorF(D2D1::ColorF::Aquamarine), fill, strokeWith);
}
