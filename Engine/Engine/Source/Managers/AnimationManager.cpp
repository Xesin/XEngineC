#include "stdafx.h"
#include "Managers\AnimationManager.h"
#include "Utils\Animation.h"

AnimationManager::AnimationManager(Sprite* parent):
	currentAnimation(NULL)
{
	this->parent = parent;
}

void AnimationManager::Update(float deltaTime) {
	if (currentAnimation) {
		currentAnimation->Update(deltaTime);
	}
}

void AnimationManager::AddAnim(LPCWSTR name, Animation* anim) {
	if (animations[name]) {
		delete animations[name];
	}
	animations[name] = anim;
	anim->SetSprite(parent);
}

void AnimationManager::PlayAnim(LPCWSTR name) {
	if (currentAnimation) {
		currentAnimation->Stop();
	}

	if (animations[name]) {
		currentAnimation = animations[name];
		currentAnimation->Play();
	}
}