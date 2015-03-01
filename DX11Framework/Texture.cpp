#include "Texture.h"

using namespace DirectX;

Texture::Texture():m_texture(nullptr){}

Texture::Texture(const Texture& other){}

Texture::~Texture(){}

bool Texture::Init(ID3D11Device* device, WCHAR* filename){
	HRESULT result;

	result = CreateDDSTextureFromFile(device, filename, NULL, &m_texture, 0, nullptr);

	return true;
}