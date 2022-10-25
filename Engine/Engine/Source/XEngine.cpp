#include "stdafx.h"
#include <stdlib.h>
#include <wchar.h>
#include <math.h>
#include <ctime>

#include "resource.h"
#include "XEngine.h"
#include "Renderer\Renderer.h"
#include "Renderer\Camera.h"
#include "GameObjects\GameObject.h"
#include "GameObjects\Rect.h"
#include "Scenes\EngineScene.h"
#include "Managers\InputManager.h"
#include "Managers\Physics.h"

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
			pDemoApp->Render();
		}
	}
}

XEngine::XEngine() 
{
	m_hwnd = NULL;
	inputManager = NULL;
}

XEngine::~XEngine()
{
	delete renderer;
	delete inputManager;
}

HRESULT XEngine::Initialize(EngineScene* initialScene, HINSTANCE instance, float resolutionX, float resolutionY)
{
	HRESULT hr;
	currentScene = initialScene;
	renderer = new Renderer();

	camera = new Camera(Vector2(resolutionX, resolutionY));
	hr = renderer->Initialize(camera);
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
		wcex.hIcon = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ENGINE));
		wcex.hCursor = LoadIcon(wcex.hInstance, IDI_APPLICATION);
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
			L"Simple Tiled Scene",
			WS_OVERLAPPEDWINDOW,
			0,
			0,
			static_cast<UINT>(ceil(resolutionX * dpiX / 96.f)),
			static_cast<UINT>(ceil(resolutionY * dpiY / 96.f)),
			NULL,
			NULL,
			instance,
			this
		);
		hr = m_hwnd ? S_OK : E_FAIL;
		if (SUCCEEDED(hr))
		{
			ShowWindow(m_hwnd, SW_SHOWNORMAL);
			UpdateWindow(m_hwnd);

			renderer->CreateDeviceResources(m_hwnd);
		}

		physics = new Physics(*renderer);
		inputManager = new InputManager(m_hwnd);
		pDemoApp = this;
		
		currentTime = GetTime();
			
		dt = 1.f / 60.f;
		currentScene->Preload();
		currentScene->Start();
		currentScene->pendingActivation = false;
	}

	return hr;
}

void XEngine::Update() {

	//Calculate semi-fixed deltaTime
	newTime = GetTime();
	double frameTime = (newTime - currentTime) * 0.001;
	currentTime = newTime;
	float fps =  1.f / (float) frameTime;
	float deltaTime = (float) min(frameTime, dt);
	if (currentScene->pendingActivation)
	{
		previous->OnDestroy();
		currentScene->Preload();
		currentScene->Start();
		currentScene->pendingActivation = false;
	}
	physics->Update(deltaTime);
	currentScene->Update(deltaTime);
}

void XEngine::Render() {
	HRESULT hr;
	hr = renderer->PreRender(m_hwnd);
	if (SUCCEEDED(hr)) {
		pDemoApp->currentScene->Render(*renderer);
		if (physics->isDebug) {
			physics->DrawDebug();
		}
		renderer->EndRender();
	}
}

void XEngine::StartScene(EngineScene * sceneToStart)
{
	CacheManager::GetInstance()->FlushCache();
	physics->ClearWorld();
	previous = currentScene;
	currentScene = sceneToStart;

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
			
				result = 0;
				wasHandled = true;
				break;
			}

			case WM_DISPLAYCHANGE:
			{
				InvalidateRect(hwnd, NULL, FALSE);
				result = 0;
				wasHandled = true;
				break;
			}

			case WM_PAINT:
			{

				ValidateRect(hwnd, NULL);
				pDemoApp->Render();
				result = 0;
				wasHandled = true;
				break;
			}

			case WM_DESTROY:
			{
				PostQuitMessage(0);

				result = 1;
				wasHandled = true;
				break;
			}

			case WM_KEYDOWN:
			{
				pDemoApp->inputManager->KeyDown((unsigned int) wParam);
				result = 0;
				wasHandled = true;
				break;
			}

			case WM_KEYUP:
			{
				pDemoApp->inputManager->KeyUp((unsigned int) wParam);
				result = 0;
				wasHandled = true;
				break;
			}

			case WM_LBUTTONDOWN:
			{
				pDemoApp->inputManager->MouseDown(MOUSE_LEFT);
				result = 0;
				wasHandled = true;
				break;
			}

			case WM_RBUTTONDOWN:
			{
				pDemoApp->inputManager->MouseDown(MOUSE_RIGHT);
				result = 0;
				wasHandled = true;
				break;
			}

			case WM_MBUTTONDOWN:
			{
				pDemoApp->inputManager->MouseDown(MOUSE_MIDDLE);
				result = 0;
				wasHandled = true;
				break;
			}

			case WM_LBUTTONUP:
			{
				pDemoApp->inputManager->MouseUp(MOUSE_LEFT);
				result = 0;
				wasHandled = true;
				break;
			}

			case WM_RBUTTONUP:
			{
				pDemoApp->inputManager->MouseUp(MOUSE_RIGHT);
				result = 0;
				wasHandled = true;
				break;
			}

			case WM_MBUTTONUP:
			{
				pDemoApp->inputManager->MouseUp(MOUSE_MIDDLE);
				result = 0;
				wasHandled = true;
				break;
			}
		}
	}

		
	if (!wasHandled)
	{
		result = DefWindowProc(hwnd, message, wParam, lParam);
	}
	

	return result;
}