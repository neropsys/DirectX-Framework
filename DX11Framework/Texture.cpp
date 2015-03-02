#include "Texture.h"

using namespace DirectX;

Texture::Texture():m_texture(nullptr){}

Texture::Texture(const Texture& other){}

Texture::~Texture(){}

bool Texture::Init(ID3D11Device* device, WCHAR* filename){
	HRESULT result;

	result = CreateDDSTextureFromFile(device, filename, NULL, &m_texture, 0, nullptr);

	if (FAILED(result)){

		OutputDebugString(L"Failed to create texture from file\r\n");

		return false;
	}

	return true;
}

void Texture::Shutdown(){
	if (m_texture){
		m_texture->Release();
		m_texture = nullptr;
	}
	return;
}

ID3D11ShaderResourceView* Texture::GetTexture(){ return m_texture; }

