#include "ModelClass.h"

using namespace DirectX;
ModelClass::ModelClass():m_vertexBuffer(0), m_indexBuffer(0){

}

ModelClass::ModelClass(const ModelClass& other){}

ModelClass::~ModelClass(){}

bool ModelClass::Init(ID3D11Device* device){
	bool result;

	result = InitBuffers(device);

	if (!result){
		OutputDebugString(L"failed to init vertex and index buffer. \r\n");
		return false;
	}

	return true;

}

void ModelClass::Shutdown(){
	ShutdownBuffers();
}

void ModelClass::Render(ID3D11DeviceContext* deviceContext){
	RenderBuffers(deviceContext);
	return;
}

int ModelClass::GetIndexCount(){
	return m_indexCount;
}

bool ModelClass::InitBuffers(ID3D11Device* device){
	VertexType* vertices;
	unsigned long* indices;

	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;

	HRESULT result;

	m_vertexCount = 4;
	m_indexCount = 6;
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
	vertices[0].color = XMFLOAT4(1.0f, 0.0f, 0.f, 1.0f);

	vertices[1].position = XMFLOAT3(-1.0f, 1.0f, 0.0f);  // Top middle.
	vertices[1].color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);

	vertices[2].position = XMFLOAT3(1.0f, -1.0f, 0.0f);  // Bottom right.
	vertices[2].color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);

	vertices[3].position = XMFLOAT3(1.0f, 1.0f, 0.0f);
	vertices[3].color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);


	indices[0] = 0;  // Bottom left.
	indices[1] = 1;  // Top middle.
	indices[2] = 2;
	indices[3] = 1;
	indices[4] = 3;
	indices[5] = 2;

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
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}


void ModelClass::ShutdownBuffers(){
	if (m_indexBuffer){
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}
	if (m_vertexBuffer){
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}
	return;
}
void ModelClass::RenderBuffers(ID3D11DeviceContext* deviceContext){
	unsigned int stride, offset;

	stride = sizeof(VertexType);
	offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;


}