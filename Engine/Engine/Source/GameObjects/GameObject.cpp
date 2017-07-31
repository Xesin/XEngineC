#include "stdafx.h"
#include "GameObjects\GameObject.h"
#include "Renderer\Renderer.h"
#include "XEngine.h"

void GameObject::SetRotation(Renderer & renderer, int width, int height)
{
	D2D1::Matrix3x2F scaleMatrix = D2D1::Matrix3x2F::Scale(scale);
	float32 angle = RADS_TO_DEGREES(transform.q.GetAngle());
	float32 x = transform.p.x;
	float32 y = transform.p.y;
	//LEFT BOTTOM COORDINATES
	D2D1::Matrix3x2F transformMatrix = D2D1::Matrix3x2F::Rotation(
		angle,
		D2D1::Point2F(
			x + width * anchor.x,
			y + height * anchor.y
		)
	);
	D2D1::Matrix3x2F translationMatrix = D2D1::Matrix3x2F::Translation(
		-width * anchor.x,
		-height * anchor.y
	);
	scaleMatrix = scaleMatrix * translationMatrix * transformMatrix;
	renderer.SetTransform(scaleMatrix);
}
