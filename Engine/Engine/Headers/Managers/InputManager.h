#pragma once
#include <stdlib.h>
#include "Utils\Delegates\DelegatesSettings.h"

#define MOUSE_LEFT 0x0
#define MOUSE_RIGHT 0x1
#define MOUSE_MIDDLE 0x2

class InputManager {

public:
	InputManager();

	void KeyDown(unsigned int vKeyCode);
	void KeyUp(unsigned int vKeyCode);

	void MouseDown(unsigned int position);
	void MouseUp(unsigned int position);

	DEFINE_MULTICAST_DELEGATE(OnKeyDown, void(unsigned int));
	DEFINE_MULTICAST_DELEGATE(OnKeyUp, void(unsigned int));
	DEFINE_MULTICAST_DELEGATE(OnMouseDown, void(unsigned int));
	DEFINE_MULTICAST_DELEGATE(OnMouseUp, void(unsigned int));

private:	


};