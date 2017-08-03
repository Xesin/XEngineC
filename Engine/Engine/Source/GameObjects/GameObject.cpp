#include "stdafx.h"
#include "GameObjects\GameObject.h"
#include "Renderer\Renderer.h"
#include "Renderer\Camera.h"
#include "Managers\ScaleManager.h"
#include "XEngine.h"

void GameObject::OnRender(Renderer & renderer)
{
	if (rigidBody != NULL) {
		transform.p = Renderer::WorldToScreenPixels(rigidBody->GetPosition());
		transform.q.Set(rigidBody->GetAngle());
	}
}

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
			(x - coreRef.camera->position.x) * renderer.scaleManager->renderTargetScaleX,
			(y - coreRef.camera->position.y) * renderer.scaleManager->renderTargetScaleY
		)
	);
	D2D1::Matrix3x2F translationMatrix = D2D1::Matrix3x2F::Translation(
		((-anchor.x) * scale.width - coreRef.camera->position.x)* renderer.scaleManager->renderTargetScaleX,
		((-anchor.y) * scale.height - coreRef.camera->position.y) * renderer.scaleManager->renderTargetScaleY
	);
	scaleMatrix = scaleMatrix * translationMatrix * transformMatrix;
	renderer.SetTransform(scaleMatrix);
}
