#pragma once
#include <map>

class Animation;
class SpriteRenderer;

class AnimationManager {
public:
	AnimationManager();
	void Update(float deltaTime);
	void AddAnim(LPCWSTR name, Animation* anim);
	void PlayAnim(LPCWSTR name);
	bool IsPlaying(LPCWSTR name);

private:
	std::map<LPCWSTR,Animation*> animations;
	Animation* currentAnimation;

public:
	SpriteRenderer* parent;
};