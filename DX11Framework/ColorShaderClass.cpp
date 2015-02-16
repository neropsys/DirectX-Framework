#include "ColorShaderClass.h"


using namespace std;
using namespace DirectX;
ColorShaderClass::ColorShaderClass():m_vertexShader(0), m_pixelShader(0), m_layout(0), m_matrixBuffer(0){


}

ColorShaderClass::ColorShaderClass(const ColorShaderClass& other){
}

ColorShaderClass::~ColorShaderClass(){
}


bool ColorShaderClass::Init(ID3D11Device* device, HWND hwnd){
	bool result;

	result = InitShader(device, hwnd, L"Color.vs", L"Color.ps");

	if (!result){
		OutputDebugString(L"failed to init pixel and vertex shader");
		return false;
	}
	return true;


}

void ColorShaderClass::Shutdown(){
	ShutdownShader();
	return;
}

bool ColorShaderClass::Render(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX& world, XMMATRIX& view, XMMATRIX& proj){
	bool result;

	result = SetShaderParameters(deviceContext, world, view, proj);
	if (!result){
		OutputDebugString(L"Failed to set shader parameter \r\n");
	}
	RenderShader(deviceContext, indexCount);

	return true;
}

bool ColorShaderClass::InitShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename){
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;

	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBufferDesc;

	errorMessage = 0;
	vertexShaderBuffer = 0;
	pixelShaderBuffer = 0;

	result = D3DCompileFromFile(vsFilename, NULL, NULL, "ColorVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
		&vertexShaderBuffer, &errorMessage);
	if (FAILED(result)){
		ProcessShaderError(L"Missing vertex shader file", hwnd, errorMessage, vsFilename);
		return false;
	}
	
	result = D3DCompileFromFile(psFilename, NULL, NULL, "ColorPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, 
		&pixelShaderBuffer, &errorMessage);
	if (FAILED(result)){
		ProcessShaderError(L"Missing pixel shader file", hwnd, errorMessage, psFilename);
		return false;
	}

	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_vertexShader);

	if (FAILED(result)){
		OutputDebugString(L"failed to create vertex shader from buffer,\r\n");
		return false;
	}

	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_pixelShader);

	if (FAILED(result)){
		OutputDebugString(L"failed to create pixel shader from buffer\r\n");
		return false;
	}
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "COLOR";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &m_layout);
	if (FAILED(result)){
		OutputDebugString(L"failed to create vertex input layout\r\n");
		return false;
	}

	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;

	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	result = device->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);

	if (FAILED(result)){
		OutputDebugString(L"failed to create constant buffer pointer\r\n");
		return false;
	}
	return true;

}
void ColorShaderClass::ProcessShaderError(LPCWSTR errorType, HWND hwnd, ID3D10Blob* errorMessage, WCHAR* fileName){
	if (errorMessage){
		OutputShaderErrorMessage(errorMessage, hwnd, fileName);
	}
	else{
		MessageBox(hwnd, fileName, errorType, MB_OK);
	}
	return;
}
void ColorShaderClass::ShutdownShader(){
	if (m_matrixBuffer){
		m_matrixBuffer->Release();
		m_matrixBuffer = 0;
	}
	if (m_layout){
		m_layout->Release();
		m_layout = 0;
	}
	if (m_vertexShader){
		m_vertexShader->Release();
		m_vertexShader = 0;
	}
	if (m_pixelShader)
	{
		m_pixelShader->Release();
		m_pixelShader = 0;
	}

	// Release the vertex shader.
	if (m_vertexShader)
	{
		m_vertexShader->Release();
		m_vertexShader = 0;
	}
	return;
}

void ColorShaderClass::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename){
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
	errorMessage = 0;

	MessageBox(hwnd, L"Error compiling shader.  Check shader-error.txt for message.", shaderFilename, MB_OK);

	return;
}

bool ColorShaderClass::SetShaderParameters(ID3D11DeviceContext* deviceContext, XMMATRIX& world, XMMATRIX& view, XMMATRIX& proj){
	HRESULT result;

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
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

	return true;
}

void ColorShaderClass::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount){
	deviceContext->IASetInputLayout(m_layout);
	deviceContext->VSSetShader(m_vertexShader, NULL, 0);
	deviceContext->PSSetShader(m_pixelShader, NULL, 0);

	deviceContext->DrawIndexed(indexCount, 0, 0);

	return;
}

