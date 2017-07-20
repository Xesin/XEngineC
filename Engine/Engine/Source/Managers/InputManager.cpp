#include "stdafx.h"
#include "Managers\InputManager.h"
#include "Renderer\Renderer.h"

InputManager::~InputManager()
{
	//delete keyDowns;
}

InputManager::InputManager()
{
	for (unsigned int i = 0; i < sizeof(keyDowns); i++) {
		keyDowns[i] = false;
	}
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
}

void InputManager::MouseUp(unsigned int position)
{
	OnMouseUp(position);
}

bool InputManager::IsDown(unsigned int vKeyCode)
{
	return keyDowns[vKeyCode];
}
