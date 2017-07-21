#include "stdafx.h"
#include "Utils\Animation.h"
#include "GameObjects\Sprite.h"

Animation::Animation(int* const frames, int refreshRate, int size) {
	this->frames = frames;
	frameCount = size;
	this->refreshRate = refreshRate;
	loopCount = 0;
}

void Animation::SetSprite(Sprite * spriteRef)
{
	sprite = spriteRef;
}

void Animation::Update(float deltaTime)
{
	if (isPlaying) {
		if (frameTime >= refreshRate) {
			currentFrame++;
			frameTime = 0 + (frameTime - refreshRate);
		}


		if (currentFrame >= frameCount) {
			if (loop) {
				currentFrame = 0;
				loopCount++;
				OnLoop(loopCount);
			}
			else {
				Stop();
				return;
			}
		}
		frameTime += (int) (deltaTime * 1000);
		animTime += (int) (deltaTime * 1000);

		sprite->currentFrame = frames[currentFrame];
	}
}

void Animation::Play()
{
	isPlaying = true;
}

void Animation::Pause()
{
	isPlaying = false;
}

void Animation::Stop()
{
	isPlaying = false;
	animTime = 0;
	currentFrame = 0;
	OnEnd();
}
