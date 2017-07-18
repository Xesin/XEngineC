#pragma once
#include "GameObjects\GameObject.h"

class CachedImage;

class Sprite : public GameObject {

public:
	Sprite(b2Vec2 spawn_position, CachedImage* image);
	void Update(float deltaTime) override {}
	void OnRender(Renderer* renderer);
public:
	CachedImage* cachedImage;
};
