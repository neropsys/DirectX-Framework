#include "Graphic.h"


using namespace DirectX;
Graphic::Graphic():
m_D3D(nullptr),
m_Camera(nullptr),
m_Model(nullptr),
m_LightShader(nullptr),
m_Light(nullptr){
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
	
	m_LightShader = new LightShader;
	if (!m_LightShader){
		OutputDebugString(L"Failed to create LightShader object\r\n");
		return false;
	}

	result = m_LightShader->Init(m_D3D->GetDevice(), hwnd);
	if (!result){
		MessageBox(hwnd, L"Could not init the LightShader object", L"Error", MB_OK);
		return false;
	}

	m_Light = new Light;
	if (!m_Light){
		OutputDebugString(L"Failed to create Light object\r\n");
		return false;
	}

	m_Light->SetDiffuseColor(1.0f, 0.0f, 1.0f, 1.0f);
	m_Light->SetDirection(0.0f, 0.0f, 1.0f);

	return true;
}

void Graphic::ShutDown(){
	if (m_LightShader)
	{
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = nullptr;
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

	if (m_Light){
		delete m_Light;
		m_Light = nullptr;
	}
	if (m_LightShader){
		delete m_LightShader;
		m_Light = nullptr;
	}

	return;
}
bool Graphic::Frame(){
	bool result;

	static float rotation = 0.0f;

	rotation += (float)XM_PI * 0.01f;

	if (rotation > 360.0f){
		rotation -= 360.0f;
	}

	result = Render(rotation);
	if (FAILED(result)){
		OutputDebugString(L"Failed to render.\r\n ");
		return false;
	}

	return true;
}

bool Graphic::Render(float rotation){
	XMMATRIX view, world, proj;
	bool result;

	m_D3D->BeginScene(0.f, 0.f, 0.f, 1.f);
	m_Camera->Render();

	m_Camera->GetViewMatrix(view);
	m_D3D->GetWorldMatrix(world);
	m_D3D->GetProjectionMatrix(proj);

	world = XMMatrixRotationY(rotation);

	m_Model->Render(m_D3D->GetDeviceContext());

	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), world, view, proj, m_Model->GetTexture(),
									m_Light->GetDirection(), m_Light->GetDiffuseColor());

	if (!result){
		OutputDebugString(L"Failed to render from colorshader class\r\n");
		return false;
	}

	m_D3D->EndScene();
	return true;
}
