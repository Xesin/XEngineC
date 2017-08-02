#include "stdafx.h"
#include "GameObjects\Circle.h"
#include "Renderer\Renderer.h"
#include "XEngine.h"

Circle::Circle(b2Vec2 spawn_pos, XEngine& ref, float32 _radiusX, float32 _radiusY, D2D1::ColorF newColor) : GameObject(spawn_pos, ref) {
	radiusX = _radiusX;
	radiusY = _radiusY;
	color = newColor;
}

void Circle::SetPhysics(bool active, PhysicBodyType bodyType, float32 friction)
{
	if (active && rigidBody == NULL) {
		//anchor.Set(0.5f, 0.5f);
		b2Vec2 worldPos = Renderer::ScreenToWorldUnits(transform.p);

		rigidBody = coreRef.physics->CreateCircleBody(worldPos, Renderer::ScreenToWorldUnits(radiusX), 1.0, friction, bodyType);
			
		float32 angle = transform.q.GetAngle();
		rigidBody->SetTransform(rigidBody->GetPosition(), angle);
	}
	else if (rigidBody != NULL) {
		DestroyBody();
	}
}

void Circle::OnRender(Renderer &renderer) {
	SetTransform(renderer, radiusX * 2.f, radiusY * 2.f);
	renderer.RenderCircle(transform.p.x, transform.p.y, radiusX, radiusY, color, scale, fill, strokeWith);
}

void Circle::Update(float deltaTime) {
	if (rigidBody != NULL) {
		transform.p = Renderer::WorldToScreenPixels(rigidBody->GetPosition());
		transform.q.Set(rigidBody->GetAngle());
	}
}
