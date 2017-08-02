#include "stdafx.h"
#include "Managers\InputManager.h"
#include "Renderer\Renderer.h"

InputManager::InputManager(HWND NewhwndInstance)
{
	for (unsigned int i = 0; i < sizeof(keyDowns); i++) {
		keyDowns[i] = false;
	}
	isMouseDown = false;
	mouseButtonDown = -1;
	hwndInstance = NewhwndInstance;
}

InputManager::~InputManager()
{
}

void InputManager::KeyDown(unsigned int vKeyCode)
{
	if (!keyDowns[vKeyCode]) {
		OnKeyDown(vKeyCode);
		keyDowns[vKeyCode] = true;
	}
	
}

void InputManager::KeyUp(unsigned int vKeyCode)
{
	OnKeyUp(vKeyCode);
	keyDowns[vKeyCode] = false;
}

void InputManager::MouseDown(unsigned int position)
{
	OnMouseDown(position);
	isMouseDown = true;
	mouseButtonDown = position;
}

void InputManager::MouseUp(unsigned int position)
{
	OnMouseUp(position);
	isMouseDown = false;
	mouseButtonDown = -1;
}

bool InputManager::IsMouseButtonDown(unsigned int position){
	return isMouseDown && position == mouseButtonDown;
}

bool InputManager::IsDown(unsigned int vKeyCode)
{
	return keyDowns[vKeyCode];
}

POINT InputManager::CursorPos() {
	POINT point;
	GetCursorPos(&point);
	ScreenToClient(hwndInstance, &point);
	
	return point;
}
