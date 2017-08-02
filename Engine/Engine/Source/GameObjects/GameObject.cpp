#include "stdafx.h"
#include "GameObjects\GameObject.h"
#include "Renderer\Renderer.h"
#include "Renderer\Camera.h"
#include "XEngine.h"

void GameObject::SetTransform(Renderer & renderer, int width, int height)
{
	D2D1_SIZE_F correctedScale = D2D1::SizeF(scale.width, scale.height * -1.f);
	D2D1::Matrix3x2F scaleMatrix = D2D1::Matrix3x2F::Scale(scale);
	float32 angle = RADS_TO_DEGREES(transform.q.GetAngle());
	float32 x = transform.p.x;
	float32 y = transform.p.y;
	//LEFT BOTTOM COORDINATES
	D2D1::Matrix3x2F transformMatrix = D2D1::Matrix3x2F::Rotation(
		angle,
		D2D1::Point2F(
			x - coreRef.camera->position.x,
			y - coreRef.camera->position.y
		)
	);
	D2D1::Matrix3x2F translationMatrix = D2D1::Matrix3x2F::Translation(
		(-anchor.x) * scale.width - coreRef.camera->position.x,
		(-anchor.y) * scale.height - coreRef.camera->position.y
	);
	scaleMatrix = scaleMatrix * translationMatrix * transformMatrix;
	renderer.SetTransform(scaleMatrix);
}
