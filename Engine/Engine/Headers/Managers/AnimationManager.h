#pragma once
#include <map>

class Animation;
class Sprite;

class AnimationManager {
public:
	AnimationManager(Sprite* parent);
	void Update(float deltaTime);
	void AddAnim(LPCWSTR name, Animation* anim);
	void PlayAnim(LPCWSTR name);
	bool IsPlaying(LPCWSTR name);

private:
	std::map<LPCWSTR,Animation*> animations;
	Animation* currentAnimation;
	Sprite* parent;
};