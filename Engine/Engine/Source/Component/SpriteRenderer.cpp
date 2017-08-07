#include "Component\SpriteRenderer.h"
#include "Component\Transform.h"
#include "Renderer\CachedImage.h"
#include "Renderer\Renderer.h"
#include "GameObjects\GameObject.h"

void SpriteRenderer::SetImage(CachedImage * image)
{
	imageToRender = image;
	frameWidth = image->Get2D2Bitmap()->GetPixelSize().width;
	frameHeight = image->Get2D2Bitmap()->GetPixelSize().height;
	columns = 1;
	rows = 1;
}

void SpriteRenderer::SetSpriteSheet(int newFrameWidth, int newFrameHeight)
{
	columns = (frameWidth / newFrameWidth);
	rows = (frameHeight / newFrameHeight);

	frameWidth = newFrameWidth;
	frameHeight = newFrameHeight;
}

void SpriteRenderer::SetAnchor(Vector2 anchor)
{
	this->anchor = anchor;
}

void SpriteRenderer::OnRender(Renderer & renderer)
{
	Transform worldTransform;
	parent->WorldTransform(&worldTransform);

	int column = currentFrame % columns;
	int row = (int)floor(((float)currentFrame / columns));

	renderer.SetTransform(worldTransform.position, Vector2((float) frameWidth, (float) frameHeight), Vector2(worldTransform.scale.x, worldTransform.scale.y), anchor, worldTransform.rotation.angles);

	renderer.RenderImage(worldTransform.position.x, worldTransform.position.y, *imageToRender, column, row, currentFrame, frameWidth, frameHeight, worldTransform.scale);
}
