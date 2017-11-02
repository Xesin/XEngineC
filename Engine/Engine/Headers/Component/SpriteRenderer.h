#pragma once
#include "Component\Component.h"
#include "Utils\MathUtils.h"

class CachedImage;
class AnimationManager;

class SpriteRenderer : public Component{

public:

	SpriteRenderer(GameObject* parent, XEngine* engineRef) : Component(parent, engineRef) {
		anchor = Vector2();
		tileSize = Vector2();
		frameWidth = 0;
		frameHeight = 0;
		currentFrame = 0;
		columns = 1;
		rows = 1;
	}

	void SetImage(CachedImage* image);

	void SetSpriteSheet(int newFrameWidth, int newFrameHeight);

	void SetAnchor(Vector2 anchor);

	void OnRender(Renderer &renderer) override;

public:
	Vector2 anchor;
	Vector2 tileSize;
	int frameWidth = 0;
	int frameHeight = 0;
	int currentFrame = 0;
	int columns = 1;
	int rows = 1;
private:
	CachedImage* imageToRender;
};