#include "stdafx.h"
#include "GameObjects\Rect.h"
#include "Renderer\Renderer.h"

Rect::Rect(b2Vec2 spawn_pos, float32 _width, float32 _height, D2D1::ColorF _color) : GameObject(spawn_pos) {
	width = _width;
	height = _height;
	color = _color;
}

void Rect::OnRender(Renderer &renderer){
	b2Transform worldPos;
	WorldTransform(worldPos);
	SetRotation(renderer, width, height);
	renderer.RenderRect(worldPos.p.x, worldPos.p.y, width, height, color, scale, fill, strokeWith);
}

//void Rect::Update(float deltaTime)
//{
//}
