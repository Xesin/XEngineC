#include "Renderer\Camera.h"

Camera::Camera(b2Vec2 worldBounds) {
	position = b2Vec2(0.f, 0.f);
	SetWorldBounds(worldBounds);
}

void Camera::SetWorldBounds(b2Vec2 worldBounds)
{
	bounds = worldBounds;
}
