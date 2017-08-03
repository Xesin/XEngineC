#pragma once

#include "Component\Component.h"
#include "Utils\MathUtils.h"

class Transform : public Component {
public:
	Transform(GameObject* parent) : Component(parent) {
		position = Vector2();
		rotation = Rotation();
	}

	Transform() : Component(){

	}

public:
	Vector2 position;
	Rotation rotation;
};