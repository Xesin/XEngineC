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
	renderer.SetTransform(transform->position, Vector2(height, width), Vector2(scale.width, scale.height), Vector2(anchor.x, anchor.y), transform->rotation.angles);
}
