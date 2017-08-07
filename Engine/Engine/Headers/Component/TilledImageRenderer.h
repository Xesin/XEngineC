#pragma once
#include "Component\Component.h"
#include "Utils\MathUtils.h"

class CachedImage;

class TilledImageRenderer : public Component {

public:
	TilledImageRenderer(GameObject* parent, XEngine* engineRef) : Component(parent, engineRef) {};

	void SetImage(CachedImage* image);

	void SetTilleSize(Vector2 size);

	void SetSpriteSheet(int newFrameWidth, int newFrameHeight);
	
	void SetAnchor(Vector2 anchor);

	void OnRender(Renderer &renderer) override;

public:
	CachedImage* imageToRender;
	Vector2 anchor;
	Vector2 tileSize;
	int frameWidth = 0;
	int frameHeight = 0;
	int currentFrame = 0;
	int columns = 1;
	int rows = 1;
};