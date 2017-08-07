#pragma once
#include "GameObjects\GameObject.h"
#include "Managers\AnimationManager.h"
#include "Managers\Physics.h"
#include "Renderer\CachedImage.h"

class SpriteRenderer;

class Sprite : public GameObject {

public:
	Sprite(Vector2 spawn_position, XEngine& ref, CachedImage &image);
	~Sprite();
	void Update(float deltaTime) override;
	void OnRender(Renderer &renderer) override;
	void SetSpriteSheet(int frameWidth, int frameHeight);

public:
	CachedImage cachedImage;
	SpriteRenderer* spriteRenderer;
	int frameWidth;
	int frameHeight;
	int currentFrame = 0;
	int columns = 1;
	int rows = 1;
	AnimationManager animationManager;
};
