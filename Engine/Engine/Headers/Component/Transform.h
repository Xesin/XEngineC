#pragma once

#include "Component\Component.h"
#include "Utils\MathUtils.h"

class Transform : public Component {
public:
	Transform(GameObject* parent) : Component(parent) {
		position = Vector2();
		rotation = Rotation();
		scale = Vector2(1.0f, 1.0f);
	}

	Transform() : Component(){

	}

public:
	Vector2 position;
	Vector2 scale;
	Rotation rotation;
};