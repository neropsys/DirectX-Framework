#include "Model.h"

using namespace DirectX;
Model::Model():m_vertexBuffer(nullptr), m_indexBuffer(nullptr){

}

Model::Model(const Model& other){}

Model::~Model(){}

bool Model::Init(ID3D11Device* device, WCHAR* textureFileName){
	bool result;

	result = InitBuffers(device);

	if (!result){
		OutputDebugString(L"Failed to init vertex and index buffer. \r\n");
		return false;
	}

	result = LoadTexture(device, textureFileName);

	if (!result){
		OutputDebugString(L"Failed to load texture from file. \r\n");

		return false;
	}


	return true;

}

void Model::Shutdown(){
	ReleaseTexture();
	ShutdownBuffers();
}

void Model::Render(ID3D11DeviceContext* deviceContext){
	RenderBuffers(deviceContext);
	return;
}

int Model::GetIndexCount(){
	return m_indexCount;
}

ID3D11ShaderResourceView* Model::GetTexture(){
	return m_Texture->GetTexture();
}

bool Model::InitBuffers(ID3D11Device* device){
	VertexType* vertices;
	unsigned long* indices;

	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;

	HRESULT result;

	m_vertexCount = 3;
	m_indexCount = 3;
	vertices = new VertexType[m_vertexCount];

	if (!vertices){
		OutputDebugString(L"failed to create vertex array");
		return false;
	}

	indices = new unsigned long[m_indexCount];

	if (!indices){
		OutputDebugString(L"failed to create indices\r\n");

		return false;
	}

	vertices[0].position = XMFLOAT3(-1.0f, -1.0f, 0.0f);
	vertices[0].texture = XMFLOAT2(0.0f, 1.0f);
	vertices[0].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

	vertices[1].position = XMFLOAT3(0.0f, 1.0f, 0.0f);  // Top middle.
	vertices[1].texture = XMFLOAT2(0.5f, 0.0f);
	vertices[1].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

	vertices[2].position = XMFLOAT3(1.0f, -1.0f, 0.0f);  // Bottom right.
	vertices[2].texture = XMFLOAT2(1.0f, 1.0f);
	vertices[2].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

	indices[0] = 0;  // Bottom left.
	indices[1] = 1;  // Top middle.
	indices[2] = 2;

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);

	if (FAILED(result)){
		OutputDebugString(L"Failed to create vertex buffer.\r\n ");
		return false;
	}

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result)){
		OutputDebugString(L"Failed to create index buffer.\r\n");
		return false;
	}

	delete[] vertices;
	vertices = nullptr;

	delete[] indices;
	indices = nullptr;

	return true;
}
bool Model::LoadTexture(ID3D11Device* device, WCHAR* filename){
	bool result;
	m_Texture = new Texture;

	if (!m_Texture){
		OutputDebugString(L"Failed to create texture object \r\n");
		return false;
	}
	result = m_Texture->Init(device, filename);

	if (!result){
		OutputDebugString(L"Failed to initialize texture object\r\n");
		return false;
	}
	return true;
}

void Model::ReleaseTexture(){
	if (m_Texture){
		m_Texture->Shutdown();
		m_Texture = nullptr;
	}
	return;
}
void Model::ShutdownBuffers(){
	if (m_indexBuffer){
		m_indexBuffer->Release();
		m_indexBuffer = nullptr;
	}
	if (m_vertexBuffer){
		m_vertexBuffer->Release();
		m_vertexBuffer = nullptr;
	}
	return;
}
void Model::RenderBuffers(ID3D11DeviceContext* deviceContext){
	unsigned int stride, offset;

	stride = sizeof(VertexType);
	offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;


}