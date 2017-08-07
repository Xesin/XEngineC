#pragma once
#include "Utils\Delegates\DelegatesSettings.h"

class SpriteRenderer;

class Animation {
public:
	Animation(int* const frames, int refreshRate, int size, bool loop = false);
	
	void SetSprite(SpriteRenderer* spriteRef);

	void Update(float deltaTime);

	void Play();

	void Pause();

	void Stop();
public:
	int* frames;
	int frameCount = 0;
	int currentFrame = 0;
	int animTime = 0;
	int refreshRate = 100;
	bool isPlaying = false;
	bool loop = false;

	DEFINE_MULTICAST_DELEGATE(OnLoop, void(int repeatCount));
	DEFINE_MULTICAST_DELEGATE(OnEnd, void());
private:
	int frameTime = 0;
	int loopCount = 0;
	SpriteRenderer* sprite;
};