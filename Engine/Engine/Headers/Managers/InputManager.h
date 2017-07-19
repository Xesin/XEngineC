#pragma once
#include <stdlib.h>

#define MOUSE_LEFT 0x0
#define MOUSE_RIGHT 0x1
#define MOUSE_MIDDLE 0x2

class InputManager {

public:
	InputManager();

	void OnKeyDown(unsigned int vKeyCode);
	void OnKeyUp(unsigned int vKeyCode);

	void OnMouseDown(unsigned int str);
	void OnMouseUp(unsigned int position);

private:	


};