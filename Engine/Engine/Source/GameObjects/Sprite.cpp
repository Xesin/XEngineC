#include "stdafx.h"
#include "GameObjects\Sprite.h"
#include "Renderer\Renderer.h"
#include "Component\SpriteRenderer.h"
#include "XEngine.h"

Sprite::Sprite(Vector2 spawn_position, XEngine& ref, CachedImage &image) : GameObject(spawn_position, ref)
{
	spriteRenderer = AddComponent<SpriteRenderer>(false, true);
	spriteRenderer->SetImage(&image);

	cachedImage = image;
	frameWidth = image.Get2D2Bitmap()->GetPixelSize().width;
	frameHeight = image.Get2D2Bitmap()->GetPixelSize().height;
	bounds.x = (float) frameWidth;
	bounds.y = (float) frameHeight;
	columns = 1;
	rows = 1;
	animationManager.parent = this;
}

Sprite::~Sprite() {

}

void Sprite::Update(float deltaTime) {
	GameObject::Update(deltaTime);
	animationManager.Update(deltaTime);
}

void Sprite::OnRender(Renderer &renderer)
{
	spriteRenderer->anchor.x = anchor.x;
	spriteRenderer->anchor.y = anchor.y;

	GameObject::OnRender(renderer);
}

void Sprite::SetSpriteSheet(int newFrameWidth, int newFrameHeight)
{
	frameHeight = newFrameHeight;
	frameWidth = newFrameWidth;
	bounds.x = (float) frameWidth;
	bounds.y = (float) frameHeight;
	spriteRenderer->SetSpriteSheet(newFrameWidth, newFrameHeight);
}
