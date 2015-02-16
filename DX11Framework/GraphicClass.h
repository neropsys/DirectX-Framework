#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_


#include <Windows.h>
#include "D3DClass.h"
#include "Camera.h"
#include "ModelClass.h"
#include "ColorShaderClass.h"
#include <DirectXMath.h>

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = .1f;

class GraphicsClass{
public:
	GraphicsClass();
	~GraphicsClass();
	GraphicsClass(const GraphicsClass&);

	bool Init(int, int, HWND);
	void ShutDown();
	bool Frame();

private:
	bool Render();
	D3DClass* m_D3D;

	Camera* m_Camera;
	ModelClass* m_Model;
	ColorShaderClass* m_ColorShader;
};

#endif