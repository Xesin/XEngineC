#include "stdafx.h"
#include "GameObjects\Sprite.h"
#include "Renderer\CachedImage.h"
#include "Renderer\Renderer.h"
#include <math.h>

Sprite::Sprite(b2Vec2 spawn_position, CachedImage * image) : GameObject(spawn_position)
{
	cachedImage = image;
	frameWidth = image->Get2D2Bitmap()->GetPixelSize().width;
	frameHeight = image->Get2D2Bitmap()->GetPixelSize().height;
	columns = 1;
	rows = 1;
	animationManager = new AnimationManager(this);
}

Sprite::~Sprite() {
	delete animationManager;
}

void Sprite::OnRender(Renderer * renderer)
{
	b2Transform worldPos;
	WorldTransform(&worldPos);
	int column = columns;

	if (column > columns - 1) {
		column = currentFrame % columns;
	}
	int row = floor(currentFrame / columns);
	renderer->RenderImage(worldPos.p.x, worldPos.p.y, cachedImage, column, row, currentFrame, frameWidth, frameHeight);
}

void Sprite::SetSpriteSheet(int newFrameWidth, int newFrameHeight)
{
	columns = (frameWidth / newFrameWidth);
	rows = (frameHeight / newFrameHeight);

	frameWidth = newFrameWidth;
	frameHeight = newFrameHeight;
}

void Sprite::Update(float deltaTime) {
	animationManager->Update(deltaTime);
}
