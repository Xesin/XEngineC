// Engine.cpp: define el punto de entrada de la aplicaci�n.
//

#include "stdafx.h"
#include <dwrite.h>
#include "Engine.h"
#include "XEngine.h"
#include "EngineScene.h"

int WINAPI WinMain(
	HINSTANCE /* hInstance */,
	HINSTANCE /* hPrevInstance */,
	LPSTR /* lpCmdLine */,
	int /* nCmdShow */
)
{
	// Use HeapSetInformation to specify that the process should
	// terminate if the heap manager detects an error in any heap used
	// by the process.
	// The return value is ignored, because we want to continue running in the
	// unlikely event that HeapSetInformation fails.
	HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);

	if (SUCCEEDED(CoInitialize(NULL)))
	{
		{
			XEngine engine;

			if (SUCCEEDED(engine.Initialize(new EngineScene(), 1280.f, 720.f)))
			{
				engine.RunMessageLoop();
			}
		}
		CoUninitialize();
	}

	return 0;
}

