#pragma once

// Windows Header Files:
#include <windows.h>

// C RunTime Header Files:

#include <d2d1.h>
#include <d2d1helper.h>
#include "ArrayList.h"

#ifndef HINST_THISCOMPONENT
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif

#ifndef Assert
#if defined( DEBUG ) || defined( _DEBUG )
#define Assert(b) do {if (!(b)) {OutputDebugStringA("Assert: " #b "\n");}} while(0)
#else
#define Assert(b)
#endif //DEBUG || _DEBUG
#endif

class Renderer;
class GameObject;
class EngineScene;

class XEngine
{
public:
	XEngine();
	~XEngine();

	// Register the window class and call methods for instantiating drawing resources
	HRESULT Initialize(EngineScene* initialScene);

	// Process and dispatch messages
	void RunMessageLoop();

private:
	// The windows procedure.
	static LRESULT CALLBACK WndProc(
		HWND hWnd,
		UINT message,
		WPARAM wParam,
		LPARAM lParam
	);

	void Update();

private:
	ArrayList<GameObject*> gameObjects;
	HWND m_hwnd;
	Renderer* renderer;
	EngineScene* currentScene;
};