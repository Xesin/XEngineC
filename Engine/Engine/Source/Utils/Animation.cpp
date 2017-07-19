#include "stdafx.h"
#include "Utils\Animation.h"
#include "GameObjects\Sprite.h"

Animation::Animation(int* frames, int refreshRate) {
	this->frames = frames;
	frameCount = sizeof(*frames);
	this->refreshRate = refreshRate;
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
			}
			else {
				Stop();
				return;
			}
		}
		frameTime += (int) (deltaTime * 1000);
		animTime += (int) (deltaTime * 1000);

		sprite->currentFrame = currentFrame;
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
}
