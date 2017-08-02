#pragma once
#include "GameObjects\GameObject.h"
#include "Managers\AnimationManager.h"
#include "Managers\Physics.h"
#include "Renderer\CachedImage.h"

class Sprite : public GameObject {

public:
	Sprite(b2Vec2 spawn_position, XEngine& ref, CachedImage &image);
	~Sprite();
	void Update(float deltaTime) override;
	void OnRender(Renderer &renderer) override;
	void SetPhysics(bool active, PhysicShape shape = PhysicShape::Box, PhysicBodyType bodyType = PhysicBodyType::Static, float32 friction = 1.0f, float32 radius = 1.0f);
	void SetSpriteSheet(int frameWidth, int frameHeight);
	void InitializeSpritePhysics(PhysicShape shape, PhysicBodyType bodyType, float32 friction, float32 radius);
private:
	virtual void SetPhysics(bool active, PhysicBodyType bodyType = PhysicBodyType::Static, float32 friction = 1.0f) override {}
public:
	CachedImage cachedImage;
	int frameWidth;
	int frameHeight;
	int currentFrame = 0;
	int columns = 1;
	int rows = 1;
	AnimationManager animationManager;
};
