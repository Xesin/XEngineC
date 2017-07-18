#pragma once

// Windows Header Files:
#include <windows.h>

// C RunTime Header Files:
#include <chrono>
#include <d2d1.h>
#include <d2d1helper.h>
#include "Utils\ArrayList.h"

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
	HRESULT Initialize(EngineScene* initialScene, float resolutionX, float resolutionY);

	// Process and dispatch messages
	void RunMessageLoop();

	static long GetTime() {
		using namespace std::chrono;

		__int64 time = system_clock::now().time_since_epoch() /
			milliseconds(1);
		return (long)time;
	}

private:
	// The windows procedure.
	static LRESULT CALLBACK WndProc(
		HWND hWnd,
		UINT message,
		WPARAM wParam,
		LPARAM lParam
	);

	void Update();

	void StartScene(EngineScene* sceneToStart);

public:
	static XEngine* instance;
private:
	ArrayList<GameObject*> gameObjects;
	HWND m_hwnd;
	Renderer* renderer;
	EngineScene* currentScene;
	unsigned long currentTime = 0;
	unsigned long newTime = 0;
	float dt = 0;
};