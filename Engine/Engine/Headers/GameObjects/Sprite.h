#pragma once
#include "GameObjects\GameObject.h"
#include "Managers\AnimationManager.h"
#include "Renderer\CachedImage.h"

class Sprite : public GameObject {

public:
	Sprite(b2Vec2 spawn_position, XEngine& ref, CachedImage &image);
	~Sprite();
	void Update(float deltaTime) override;
	void OnRender(Renderer &renderer) override;
	virtual void SetPhysics(bool active, bool dynamic = false) override;
	void SetSpriteSheet(int frameWidth, int frameHeight);

public:
	CachedImage cachedImage;
	int frameWidth;
	int frameHeight;
	int currentFrame = 0;
	int columns = 1;
	int rows = 1;
	AnimationManager animationManager;
};
