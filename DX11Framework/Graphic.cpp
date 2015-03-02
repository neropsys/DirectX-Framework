#include "Graphic.h"


using namespace DirectX;
Graphic::Graphic():m_D3D(nullptr), m_Camera(nullptr), m_Model(nullptr), m_TextureShader(nullptr){
}

Graphic::Graphic(const Graphic& other){}

Graphic::~Graphic(){}

bool Graphic::Init(int screenWidth, int screenHeight, HWND hwnd){

	bool result;

	m_D3D = new D3D;
	if (!m_D3D) return false;

	result = m_D3D->Init(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result){
		MessageBox(hwnd, L"Cant init d3d", L"Error", MB_OK);
		return false;

	}

	m_Camera = new Camera;
	if (!m_Camera){
		OutputDebugString(L"Failed to init camera\r\n");
		return false;
	}

	m_Camera->SetPos(0.f, 0.f, -10.f);

	m_Model = new Model;

	if (!m_Model){
		OutputDebugString(L"Failed to create model object\r\n");
		return false;
	}

	result = m_Model->Init(m_D3D->GetDevice(), L"seafloor.dds");
	if (!result){
		MessageBox(hwnd, L"Could not init the model object", L"Error", MB_OK);
		return false;
	}
	
	m_TextureShader = new TextureShader;
	if (!m_TextureShader){
		OutputDebugString(L"Failed to create TextureShader object\r\n");
		return false;
	}

	result = m_TextureShader->Init(m_D3D->GetDevice(), hwnd);
	if (!result){
		MessageBox(hwnd, L"Could not init the TextureShader object", L"Error", MB_OK);
		return false;
	}

	return true;
}

void Graphic::ShutDown(){
	if (m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = nullptr;
	}

	if (m_Model)
	{
		m_Model->Shutdown();
		delete m_Model;
		m_Model = nullptr;
	}

	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = nullptr;
	}

	if (m_D3D){
		m_D3D->ShutDown();
		delete m_D3D;
		m_D3D = nullptr;
	}
	return;
}
bool Graphic::Frame(){
	bool result;

	result = Render();
	if (FAILED(result)){
		OutputDebugString(L"Failed to render.\r\n ");
		return false;
	}

	return true;
}

bool Graphic::Render(){
	XMMATRIX view, world, proj;
	bool result;

	m_D3D->BeginScene(0.f, 0.f, 1.f, 1.f);
	m_Camera->Render();

	m_Camera->GetViewMatrix(view);
	m_D3D->GetWorldMatrix(world);
	m_D3D->GetProjectionMatrix(proj);

	m_Model->Render(m_D3D->GetDeviceContext());

	result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), world, view, proj, m_Model->GetTexture());

	if (!result){
		OutputDebugString(L"Failed to render from colorshader class\r\n");
		return false;
	}

	m_D3D->EndScene();
	return true;
}
