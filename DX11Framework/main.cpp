#include "System.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdShow){
	System* system;
	bool result;


	// Create the system object.
	system = new System;
	if (!system)
	{
		OutputDebugString(L"Failed to create systemclass.\r\n ");
		return 0;
	}

	// Initialize and run the system object.
	result = system->Init();
	if (result)
	{
		system->Run();
	}

	// Shutdown and release the system object.
	system->Shutdown();
	delete system;
	system = nullptr;

	return 0;
}