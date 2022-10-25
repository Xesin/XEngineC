#pragma once
#include "Utils\Delegates\DelegatesSettings.h"
#include "Utils\Delegates\MultiCastDelegate.h"

class SpriteRenderer;

DECLARE_MULTICAST_DELEGATE(OnLoopDelegate, int)
DECLARE_MULTICAST_DELEGATE(OnEndDelegate, int)

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
	OnLoopDelegate OnLoop;
	OnLoopDelegate OnEnd;
private:
	int frameTime = 0;
	int loopCount = 0;
	SpriteRenderer* sprite;
};