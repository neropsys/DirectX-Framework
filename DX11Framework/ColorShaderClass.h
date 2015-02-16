#ifndef _COLORSHADERCLASS_H_
#define _COLORSHADERCLASS_H_

#pragma comment(lib, "D3DCompiler.lib")
#include <d3d11.h>
#include <DirectXMath.h>
#include <fstream>
#include <d3dcompiler.h>

class ColorShaderClass{
private:
	struct MatrixBufferType{
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX proj;
	};

public:
	ColorShaderClass();
	~ColorShaderClass();
	ColorShaderClass(const ColorShaderClass&);

	bool Init(ID3D11Device*, HWND);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int, DirectX::XMMATRIX& world, DirectX::XMMATRIX& view, DirectX::XMMATRIX& proj);

private:
	bool InitShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

	bool SetShaderParameters(ID3D11DeviceContext*, DirectX::XMMATRIX& world, DirectX::XMMATRIX& view, DirectX::XMMATRIX& proj);
	void RenderShader(ID3D11DeviceContext*, int);

	void ProcessShaderError(LPCWSTR, HWND, ID3D10Blob*, WCHAR*);
private:
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matrixBuffer;
};



#endif