#include "LightShader.h"


using namespace std;
using namespace DirectX;
LightShader::LightShader():
m_vertexShader(nullptr), 
m_pixelShader(nullptr), 
m_layout(nullptr),
m_matrixBuffer(nullptr),
m_sampleState(nullptr), 
m_lightBuffer(nullptr){

}

LightShader::LightShader(const LightShader& other){
}

LightShader::~LightShader(){
}


bool LightShader::Init(ID3D11Device* device, HWND hwnd){
	bool result;

	result = InitShader(device, hwnd, L"Light.vs", L"Light.ps");

	if (!result){
		OutputDebugString(L"failed to init pixel and vertex shader");
		return false;
	}
	return true;


}

void LightShader::Shutdown(){
	ShutdownShader();
	return;
}

bool LightShader::Render(ID3D11DeviceContext* deviceContext, int indexCount, 
	XMMATRIX& world, 
	XMMATRIX& view, 
	XMMATRIX& proj, 
	ID3D11ShaderResourceView* texture,
	XMFLOAT3 lightDirection,
	XMFLOAT4 diffuseColor){
	bool result;

	result = SetShaderParameters(deviceContext, world, view, proj, texture, lightDirection, diffuseColor);
	if (!result){
		OutputDebugString(L"Failed to set shader parameter \r\n");
	}
	RenderShader(deviceContext, indexCount);

	return true;
}

bool LightShader::InitShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename){
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;

	D3D11_INPUT_ELEMENT_DESC polygonLayout[3];
	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_SAMPLER_DESC samplerDesc;

	D3D11_BUFFER_DESC lightBufferDesc;

	errorMessage = nullptr;
	vertexShaderBuffer = nullptr;
	pixelShaderBuffer = nullptr;

	result = D3DCompileFromFile(vsFilename, nullptr, nullptr, "LightVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
		&vertexShaderBuffer, &errorMessage);
	if (FAILED(result)){
		ProcessShaderError(L"Missing vertex shader file", hwnd, errorMessage, vsFilename);
		return false;
	}
	
	result = D3DCompileFromFile(psFilename, nullptr, nullptr, "LightPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, 
		&pixelShaderBuffer, &errorMessage);
	if (FAILED(result)){
		ProcessShaderError(L"Missing pixel shader file", hwnd, errorMessage, psFilename);
		return false;
	}

	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), nullptr, &m_vertexShader);

	if (FAILED(result)){
		OutputDebugString(L"failed to create vertex shader from buffer,\r\n");
		return false;
	}

	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), nullptr, &m_pixelShader);

	if (FAILED(result)){
		OutputDebugString(L"failed to create pixel shader from buffer\r\n");
		return false;
	}

	//Input layout
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "TEXCOORD";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	polygonLayout[2].SemanticName = "NORMAL";
	polygonLayout[2].SemanticIndex = 0;
	polygonLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[2].InputSlot = 0;
	polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[2].InstanceDataStepRate = 0;

	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &m_layout);
	if (FAILED(result)){
		OutputDebugString(L"failed to create vertex input layout\r\n");
		return false;
	}

	vertexShaderBuffer->Release();
	vertexShaderBuffer = nullptr;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = nullptr;

	//createing texture sampler state description
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	result = device->CreateSamplerState(&samplerDesc, &m_sampleState);

	if (FAILED(result)){
		OutputDebugString(L"Failed to create texture sampler state.\r\n");
		return false;
	}
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	result = device->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);

	if (FAILED(result)){
		OutputDebugString(L"Failed to reate constant buffer pointer(Matrix)\r\n");
		return false;
	}

	lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightBufferDesc.ByteWidth = sizeof(LightBufferType);
	lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc.MiscFlags = 0;
	lightBufferDesc.StructureByteStride = 0;

	result = device->CreateBuffer(&lightBufferDesc, nullptr, &m_lightBuffer);

	if (FAILED(result)){

		OutputDebugString(L"Failed to crate constant bufferpointer(Light)\r\n");
		return false;
	}


	return true;

}
void LightShader::ProcessShaderError(LPCWSTR errorType, HWND hwnd, ID3D10Blob* errorMessage, WCHAR* fileName){
	if (errorMessage){
		OutputShaderErrorMessage(errorMessage, hwnd, fileName);
	}
	else{
		MessageBox(hwnd, fileName, errorType, MB_OK);
	}
	return;
}
void LightShader::ShutdownShader(){

	if (m_sampleState){
		m_sampleState->Release();
		m_sampleState = nullptr;
	}

	if (m_matrixBuffer){
		m_matrixBuffer->Release();
		m_matrixBuffer = nullptr;
	}
	if (m_layout){
		m_layout->Release();
		m_layout = nullptr;
	}
	if (m_vertexShader){
		m_vertexShader->Release();
		m_vertexShader = nullptr;
	}
	if (m_pixelShader)
	{
		m_pixelShader->Release();
		m_pixelShader = nullptr;
	}

	// Release the vertex shader.
	if (m_vertexShader)
	{
		m_vertexShader->Release();
		m_vertexShader = nullptr;
	}
	if (m_lightBuffer){
		m_lightBuffer->Release();
		m_lightBuffer = nullptr;
	}

	return;
}

void LightShader::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename){
	char* compileErrors;
	unsigned long bufferSize, i;
	ofstream fout;

	compileErrors = (char*)(errorMessage->GetBufferPointer());
	bufferSize = errorMessage->GetBufferSize();

	fout.open("shader-error.txt");

	for (i = 0; i < bufferSize; i++){
		fout << compileErrors[i];
	}
	fout.close();

	errorMessage->Release();
	errorMessage = nullptr;

	MessageBox(hwnd, L"Error compiling shader.  Check shader-error.txt for message.", shaderFilename, MB_OK);

	return;
}

bool LightShader::SetShaderParameters(
	ID3D11DeviceContext* deviceContext, 
	XMMATRIX& world, 
	XMMATRIX& view, 
	XMMATRIX& proj, 
	ID3D11ShaderResourceView* texture,
	XMFLOAT3 lightDirection,
	XMFLOAT4 diffuseColor
	){

	HRESULT result;

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	LightBufferType* dataPtr2;
	unsigned int bufferNumber;

	world = XMMatrixTranspose(world);
	view = XMMatrixTranspose(view);
	proj = XMMatrixTranspose(proj);

	result = deviceContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	if (FAILED(result)){
		OutputDebugString(L"Failed to lock constant buffer\r\n");
		return false;
	}

	dataPtr = (MatrixBufferType*)mappedResource.pData;

	dataPtr->world = world;
	dataPtr->view = view;
	dataPtr->proj = proj;

	deviceContext->Unmap(m_matrixBuffer, 0);

	bufferNumber = 0;

	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);

	deviceContext->PSSetShaderResources(0, 1, &texture);

	result = deviceContext->Map(m_lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result)){
		OutputDebugString(L"Failed to lock light constant buffer\r\n");
		return false;
	}

	dataPtr2 = (LightBufferType*)mappedResource.pData;

	dataPtr2->diffuseColor = diffuseColor;
	dataPtr2->lightDirection = lightDirection;
	dataPtr2->padding = 0.0f;

	deviceContext->Unmap(m_lightBuffer, 0);

	bufferNumber = 0;

	deviceContext->PSSetConstantBuffers(bufferNumber, 1, &m_lightBuffer);

	return true;
}

void LightShader::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount){
	deviceContext->IASetInputLayout(m_layout);

	deviceContext->VSSetShader(m_vertexShader, nullptr, 0);
	deviceContext->PSSetShader(m_pixelShader, nullptr, 0);

	deviceContext->PSSetSamplers(0, 1, &m_sampleState);

	deviceContext->DrawIndexed(indexCount, 0, 0);

	return;
}

