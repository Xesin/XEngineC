#pragma once

class Sprite;

class Animation {
public:
	Animation(int* frames, int refreshRate);
	
	void SetSprite(Sprite* spriteRef);

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
private:
	int frameTime = 0;
	Sprite* sprite;
};