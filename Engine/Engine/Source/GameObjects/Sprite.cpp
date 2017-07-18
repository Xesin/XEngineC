#include "stdafx.h"
#include "GameObjects\Sprite.h"
#include "Renderer\CachedImage.h"
#include "Renderer\Renderer.h"

Sprite::Sprite(b2Vec2 spawn_position, CachedImage * image) : GameObject(spawn_position)
{
	cachedImage = image;
}

void Sprite::OnRender(Renderer * renderer)
{
	b2Transform worldPos;
	WorldTransform(&worldPos);
	renderer->RenderImage(worldPos.p.x, worldPos.p.y, cachedImage);
}
