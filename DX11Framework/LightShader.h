#ifndef _COLORSHADER_H_
#define _COLORSHADER_H_

#pragma comment(lib, "D3DCompiler.lib")
#pragma comment(lib, "d3d11.lib")
#include <d3d11.h>
#include <DirectXMath.h>
#include <fstream>
#include <d3dcompiler.h>

class LightShader{
private:
	struct MatrixBufferType{
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX proj;
	};
	struct LightBufferType{
		DirectX::XMFLOAT4 diffuseColor;
		DirectX::XMFLOAT3 lightDirection;
		float padding;
	};
public:
	LightShader();
	~LightShader();
	LightShader(const LightShader&);

	bool Init(ID3D11Device*, HWND);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int, 
		DirectX::XMMATRIX& world, 
		DirectX::XMMATRIX& view, 
		DirectX::XMMATRIX& proj, 
		ID3D11ShaderResourceView*,
		DirectX::XMFLOAT3 lightDirection,
		DirectX::XMFLOAT4 diffuseColor);

private:
	bool InitShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

	bool SetShaderParameters(ID3D11DeviceContext*, 
		DirectX::XMMATRIX& world, 
		DirectX::XMMATRIX& view,
		DirectX::XMMATRIX& proj, 
		ID3D11ShaderResourceView*,
		DirectX::XMFLOAT3 lightDirection,
		DirectX::XMFLOAT4 diffuseColor
		);
	void RenderShader(ID3D11DeviceContext*, int);

	void ProcessShaderError(LPCWSTR, HWND, ID3D10Blob*, WCHAR*);
private:
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matrixBuffer;

	ID3D11SamplerState* m_sampleState;

	ID3D11Buffer* m_lightBuffer;
};



#endif