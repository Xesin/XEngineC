#pragma once
#include "GameObjects\GameObject.h"

class CachedImage;
class Animation;

class Sprite : public GameObject {

public:
	Sprite(b2Vec2 spawn_position, CachedImage* image);
	void Update(float deltaTime) override;
	void OnRender(Renderer* renderer);
	void SetSpriteSheet(int frameWidth, int frameHeight);
public:
	CachedImage* cachedImage;
	int frameWidth;
	int frameHeight;
	int currentFrame = 0;
	int columns = 1;
	int rows = 1;

private:
	Animation* animManager;
};
