#include "stdafx.h"
#include "GameObjects\GameObject.h"
#include "Renderer\Renderer.h"
#include "Renderer\Camera.h"
#include "Managers\ScaleManager.h"
#include "XEngine.h"

void GameObject::OnRender(Renderer & renderer)
{
	if (rigidBody != NULL) {
		b2Vec2 bodyPos = rigidBody->GetPosition();
		transform->position = Renderer::WorldToScreenPixels(Vector2(bodyPos.x, bodyPos.y));
		transform->rotation.angles = RADS_TO_DEGREES(rigidBody->GetAngle());
	}

	std::map<std::type_index, Component*>::iterator it = componentsMap.begin();
	while(it != componentsMap.end()) {
		Component* componentToCheck = it->second;
		if (componentToCheck != nullptr && componentToCheck->markedToDestroy) {
			componentsMap.erase(it->first);
			delete componentToCheck;
		}
		else if (componentToCheck->mustRender) {
			componentToCheck->OnRender(renderer);
			++it;
		}
		else {
			++it;
		}
	}
}

void GameObject::SetTransform(Renderer & renderer, int width, int height)
{
	D2D1_SIZE_F correctedScale = D2D1::SizeF(scale.width, scale.height * -1.f);
	D2D1::Matrix3x2F scaleMatrix = D2D1::Matrix3x2F::Scale(scale);
	float32 angle = transform->rotation.angles;
	float32 x = transform->position.x;
	float32 y = transform->position.y;
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
