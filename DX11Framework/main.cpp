#include "SystemClass.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdShow){
	SystemClass* System;
	bool result;


	// Create the system object.
	System = new SystemClass;
	if (!System)
	{
		OutputDebugString(L"Failed to create systemclass.\r\n ");
		return 0;
	}

	// Initialize and run the system object.
	result = System->Init();
	if (result)
	{
		System->Run();
	}

	// Shutdown and release the system object.
	System->Shutdown();
	delete System;
	System = 0;

	return 0;
}