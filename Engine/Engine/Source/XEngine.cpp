#include "stdafx.h"
#include <stdlib.h>
#include <wchar.h>
#include <math.h>
#include <ctime>


#include "XEngine.h"
#include "Renderer\Renderer.h"
#include "GameObjects\GameObject.h"
#include "GameObjects\Rect.h"
#include "Scenes\EngineScene.h"

XEngine* pDemoApp;

void XEngine::RunMessageLoop()
{
	MSG msg;
	msg.message = WM_NULL;
	while (msg.message != WM_QUIT) {
		if (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE)) {
			
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			pDemoApp->Update();
			pDemoApp->renderer->PreRender();
			pDemoApp->currentScene->Render(m_hwnd, pDemoApp->renderer);
			pDemoApp->renderer->EndRender();
		}
	}
}

XEngine::XEngine():
	m_hwnd(NULL)
{
}
XEngine::~XEngine()
{
	delete renderer;
}


HRESULT XEngine::Initialize(EngineScene* initialScene, float resolutionX, float resolutionY)
{
	HRESULT hr;
	currentScene = initialScene;
	renderer = new Renderer();
	// as the Direct2D factory.
	hr = renderer->Initialize();
	if (SUCCEEDED(hr))
	{
		// Register the window class.
		WNDCLASSEX wcex = { sizeof(WNDCLASSEX) };
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = XEngine::WndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = sizeof(LONG_PTR);
		wcex.hInstance = HINST_THISCOMPONENT;
		wcex.hbrBackground = NULL;
		wcex.lpszMenuName = NULL;
		wcex.hCursor = LoadCursor(NULL, IDI_APPLICATION);
		wcex.lpszClassName = L"D2DDemoApp";

		RegisterClassEx(&wcex);


		// Because the CreateWindow function takes its size in pixels,
		// obtain the system DPI and use it to scale the window size.
		FLOAT dpiX, dpiY;

		// The factory returns the current system DPI. This is also the value it will use
		// to create its own windows.
		renderer->GetDesktopDpi(&dpiX, &dpiY);


		// Create the window.
		m_hwnd = CreateWindow(
			L"D2DDemoApp",
			L"Direct2D Demo App",
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			static_cast<UINT>(ceil(resolutionX * dpiX / 96.f)),
			static_cast<UINT>(ceil(resolutionY * dpiY / 96.f)),
			NULL,
			NULL,
			HINST_THISCOMPONENT,
			this
		);
		hr = m_hwnd ? S_OK : E_FAIL;
		if (SUCCEEDED(hr))
		{
			ShowWindow(m_hwnd, SW_SHOWNORMAL);
			UpdateWindow(m_hwnd);

			renderer->CreateDeviceResources(m_hwnd);
		}

		pDemoApp = this;
		
		currentTime = GetTime();
			
		dt = 1.f / 60.f;
		currentScene->Preload();
		currentScene->Start();
	}

	return hr;
}

void XEngine::Update() {

	//Calculate semi-fixed deltaTime
	newTime = GetTime();
	double frameTime = (newTime - currentTime) * 0.001;
	currentTime = newTime;
	float deltaTime = (float) min(frameTime, dt);

	currentScene->Update(deltaTime);
}

void XEngine::StartScene(EngineScene * sceneToStart)
{
	currentScene->OnDestroy();
	CacheManager::GetInstance()->FlushCache();
	currentScene = sceneToStart;
	currentScene->Preload();
	currentScene->Start();
}

LRESULT CALLBACK XEngine::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;

	bool wasHandled = false;

	if (pDemoApp)
	{
		switch (message)
		{
		case WM_SIZE:
		{
			UINT width = LOWORD(lParam);
			UINT height = HIWORD(lParam);
			pDemoApp->renderer->OnResize(width, height);
		}
		result = 0;
		wasHandled = true;
		break;

		case WM_DISPLAYCHANGE:
		{
			InvalidateRect(hwnd, NULL, FALSE);
		}
		result = 0;
		wasHandled = true;
		break;

		case WM_PAINT:
		{
				
			ValidateRect(hwnd, NULL);
			pDemoApp->currentScene->Render(hwnd, pDemoApp->renderer);
		}
		result = 0;
		wasHandled = true;
		break;

		case WM_DESTROY:
		{
			PostQuitMessage(0);
		}
		result = 1;
		wasHandled = true;
		break;
		}
	}

		
	if (!wasHandled)
	{
		result = DefWindowProc(hwnd, message, wParam, lParam);
	}
	

	return result;
}