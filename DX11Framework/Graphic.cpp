#include "Graphic.h"


using namespace DirectX;
Graphic::Graphic():m_D3D(0), m_Camera(0), m_Model(0), m_ColorShader(0){
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

	result = m_Model->Init(m_D3D->GetDevice());
	if (!result){
		MessageBox(hwnd, L"Could not init the model object", L"Error", MB_OK);
		return false;
	}
	
	m_ColorShader = new ColorShader;
	if (!m_ColorShader){
		OutputDebugString(L"Failed to create colorshader object\r\n");
		return false;
	}

	result = m_ColorShader->Init(m_D3D->GetDevice(), hwnd);
	if (!result){
		MessageBox(hwnd, L"Could not init the color shader object", L"Error", MB_OK);
		return false;
	}

	return true;
}

void Graphic::ShutDown(){
	if (m_ColorShader)
	{
		m_ColorShader->Shutdown();
		delete m_ColorShader;
		m_ColorShader = 0;
	}

	if (m_Model)
	{
		m_Model->Shutdown();
		delete m_Model;
		m_Model = 0;
	}

	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	if (m_D3D){
		m_D3D->ShutDown();
		delete m_D3D;
		m_D3D = 0;
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

	result = m_ColorShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), world, view, proj);

	if (!result){
		OutputDebugString(L"Failed to render from colorshader class\r\n");
		return false;
	}

	m_D3D->EndScene();
	return true;
}
