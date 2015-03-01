#include "System.h"

System::System():m_input(nullptr), m_Graphics(nullptr){
}

System::System(const System& other){
}

System::~System(){

}

bool System::Init(){
	int screenWidth, screenHeight;
	bool result;

	screenWidth = 0;
	screenHeight = 0;

	InitWindows(screenWidth, screenHeight);
	m_input = new Input;
	if (!m_input){ 
		OutputDebugString(L"Failed to create Input class.\r\n");
		return false; 
	}
	m_input->Init();

	m_Graphics = new Graphic;
	if (!m_Graphics){
		OutputDebugString(L"Failed to create Graphic class.\r\n ");
		return false;
	}

	result = m_Graphics->Init(screenWidth, screenHeight, m_hwnd);

	if (!result){
		OutputDebugString(L"Failed to init graphic class. ");
		return false;
	}

	return true;

}

void System::Shutdown(){
	if (m_Graphics){
		m_Graphics->ShutDown();
		delete m_Graphics;
		m_Graphics = nullptr;

	}
	if (m_input){
		delete m_input;
		m_input = nullptr;
	}
	ShutdownWindows();
}

void System::Run(){
	MSG msg;
	bool done, result;

	ZeroMemory(&msg, sizeof(MSG));

	done = false;
	while (!done){
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){
			TranslateMessage(&msg);
			DispatchMessage(&msg);

		}
		if (msg.message == WM_QUIT) done = true;
		else
		{
			result = Frame();
			if (!result) done = true;
		}
	}
	return;

}

bool System::Frame(){
	bool result;
	if (m_input->IsKeyDown(VK_ESCAPE))
		return false;

	result = m_Graphics->Frame();
	if (!result){
		OutputDebugString(L"Failed to create frame.\r\n ");
		return false; 
	}

	return true;
}

LRESULT CALLBACK System::MessageHandler(HWND m_hwnd, UINT umsg, WPARAM wparam, LPARAM lparam){
	switch (umsg){
	case WM_KEYDOWN:{
		m_input->KeyDown((unsigned int)wparam);
		return 0;
	}
	case WM_KEYUP:{
		m_input->KeyUp((unsigned int)wparam);
		return 0;
	}
	default:{
		return DefWindowProc(m_hwnd, umsg, wparam, lparam);

	}

	}
}

void System::InitWindows(int& screenWidth, int& screenHeight){

	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;

	ApplicationHandle = this;

	m_hinstance = GetModuleHandle(NULL);

	m_applicationName = L"Engine";

	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hinstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	RegisterClassEx(&wc);

	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	if (FULL_SCREEN){
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// Set the position of the window to the top left corner.
		posX = posY = 0;
	}
	else{
		// If windowed then set it to 800x600 resolution.
		screenWidth = 800;
		screenHeight = 600;

		// Place the window in the middle of the screen.
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) * .5f;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) * .5f;
	}

	// Create the window with the screen settings and get the handle to it.
	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		posX, posY, screenWidth, screenHeight, NULL, NULL, m_hinstance, NULL);

	// Bring the window up on the screen and set it as main focus.
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	// Hide the mouse cursor.
	ShowCursor(false);

	return;
}

void System::ShutdownWindows(){
	ShowCursor(true);
	if (FULL_SCREEN){
		ChangeDisplaySettings(NULL, 0);

	}
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = NULL;

	ApplicationHandle = NULL;

	return;

}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam){
	switch (umessage){
	case WM_DESTROY:{
		PostQuitMessage(0);
		return 0;
	}
	case WM_CLOSE:{
		PostQuitMessage(0);
		return 0;
	}
	default:
		return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
	}
}