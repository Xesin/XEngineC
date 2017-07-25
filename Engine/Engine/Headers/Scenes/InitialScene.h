#pragma once
#include "EngineScene.h"

class InitialScene : public EngineScene {

public:
	void OnKeyDown(unsigned int);
	void Start() override;
	void Update(float deltaTime) override;
};