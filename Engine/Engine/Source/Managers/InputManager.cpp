#include "stdafx.h"
#include "Managers\InputManager.h"
#include "Renderer\Renderer.h"

InputManager::InputManager()
{
}

void InputManager::KeyDown(unsigned int vKeyCode)
{
	OnKeyDown(vKeyCode);
}

void InputManager::KeyUp(unsigned int vKeyCode)
{

	OnKeyUp(vKeyCode);
}

void InputManager::MouseDown(unsigned int position)
{
	OnMouseDown(position);
}

void InputManager::MouseUp(unsigned int position)
{
	OnMouseUp(position);
}
