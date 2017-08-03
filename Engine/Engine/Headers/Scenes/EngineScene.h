#pragma once
#include "Utils\ArrayList.h"
#include "GameObjects\GameObject.h"
#include "Renderer\Renderer.h"
#include "Managers\CacheManager.h"


class EngineScene {

public:
	EngineScene(XEngine& ref):coreRef(ref)
	{
	}

	~EngineScene() {
		renderList.empty();
		updateList.empty();
	}

	void Preload() {}

	virtual void Start() = 0;
	virtual void Update(float deltaTime) {
		for (int i = updateList.size - 1 ; i >= 0; i--) {
			GameObject& go = *updateList[i];
			if (go.isPendingDestroy) {
				delete updateList[i];
				updateList.erase(i);
			}
			else {
				go.Update(deltaTime);
			}
			
		}

	}
	virtual void OnDestroy() = 0;
	void Render(Renderer &renderer) {
		renderer.OnRenderGroup(renderList);
	}
protected:
	void AddGameObject(GameObject* go, bool mustUpdate = true, bool mustRender = true) {
		
		if (mustUpdate) {
			updateList.insert(go);
		}

		if (mustRender) {
			renderList.insert(go);
		}
	}

public:
	bool pendingActivation = true;
	ArrayList<GameObject*> renderList;
	ArrayList<GameObject*> updateList;

protected:
	XEngine& coreRef;
};