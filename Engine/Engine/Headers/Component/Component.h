#pragma once

class GameObject;
class Renderer;
class Transform;

class Component {
public:
	Component(GameObject* parent) :
		parent(parent)
	{
	}

	Component()
	{
	}

	virtual void OnEnable() {}
	virtual void OnDisable() {}
	virtual void OnDestroy() {}

	virtual void Start() {
		started = true;
	}

	virtual void Update() {
		if (enabled && !started) {
			Start();
		}
	}

	virtual void OnRender(Renderer& renderer) {}

	void Enable() {
		if (!enabled && !markedToDestroy) {
			enabled = true;
			OnEnable();
		}
	}

	void Disable() {
		if (enabled && !markedToDestroy) {
			enabled = false;
			OnDisable();
		}
	}

	void Destroy() {
		if (!markedToDestroy) {
			OnDisable();
			markedToDestroy = true;
		}
	}

	bool IsValid() {
		return enabled == true;
	}

	bool IsEnabled() {
		return enabled;
	}

public:
	bool markedToDestroy = false;
	bool mustUpdate = true;
	bool mustRender = false;
	GameObject* parent;
private:
	bool validObject = true;
	bool enabled = true;
	bool started = false;
};