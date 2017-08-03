#include "Component\TilledImageRenderer.h"
#include "Component\Transform.h"
#include "Renderer\CachedImage.h"
#include "Renderer\Renderer.h"
#include "GameObjects\GameObject.h"


void TilledImageRenderer::SetImage(CachedImage * image) {
	imageToRender = image;
	frameWidth = image->Get2D2Bitmap()->GetPixelSize().width;
	frameHeight = image->Get2D2Bitmap()->GetPixelSize().height;
}

void TilledImageRenderer::SetTilleSize(Vector2 size)
{
	tileSize = size;
}

void TilledImageRenderer::SetSpriteSheet(int newFrameWidth, int newFrameHeight)
{
	columns = (frameWidth / newFrameWidth);
	rows = (frameHeight / newFrameHeight);

	frameWidth = newFrameWidth;
	frameHeight = newFrameHeight;
}

void TilledImageRenderer::SetAnchor(Vector2 anchor)
{
	this->anchor = anchor;
}

void TilledImageRenderer::OnRender(Renderer & renderer)
{
	Component::OnRender(renderer);
	Transform worldPos;
	parent->WorldTransform(&worldPos);
	int column = currentFrame % columns;
	int row = (int)floor(((float)currentFrame / columns));

	renderer.RenderTilledImage(worldPos.position, *imageToRender, column, row, currentFrame, Vector2(frameWidth, frameHeight), Vector2(), tileSize, anchor);
}
