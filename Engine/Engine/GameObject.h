#pragma once

class Renderer;

class GameObject {
public:
	virtual void OnRender(Renderer* renderer) const = 0;
};
