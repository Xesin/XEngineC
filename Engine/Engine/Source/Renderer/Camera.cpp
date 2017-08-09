#include "Renderer\Camera.h"

Camera::Camera(Vector2 worldBounds) {
	position = Vector2(0.f, 0.f);
	SetWorldBounds(worldBounds);
}

void Camera::SetWorldBounds(Vector2 worldBounds)
{
	bounds = worldBounds;
}
