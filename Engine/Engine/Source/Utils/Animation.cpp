#include "stdafx.h"
#include "Utils\Animation.h"
#include "Component\SpriteRenderer.h"

Animation::Animation(int* const framesRef, int refreshRate, int size, bool loop) {
	frames = new int[size];
	for (int i = 0; i < size; i++) {
		frames[i] = framesRef[i];
	}
	frameCount = size;
	this->refreshRate = refreshRate;
	this->loop = loop;
	loopCount = 0;
}

void Animation::SetSprite(SpriteRenderer * spriteRef)
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
