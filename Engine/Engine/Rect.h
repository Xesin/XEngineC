#pragma once
#include "GameObject.h"
class Rect : public GameObject {
public:
	void OnRender(Renderer* renderer) const;


public:
	float x = 0;
};
