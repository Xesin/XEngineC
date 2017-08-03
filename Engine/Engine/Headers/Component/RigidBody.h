#pragma once

#include "Component\Component.h"

class RigidBody : public Component {

public:
	RigidBody(GameObject* parent) : Component(parent) {
		
	}
};