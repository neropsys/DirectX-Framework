#ifndef _TEXTURECLASS_H_
#define _TEXTURECLASS_H_

#include <d3d11.h>
#include <DDSTextureLoader.h>
class Texture{
public: 
	Texture();
	Texture(const Texture&);
	~Texture();

	bool Init(ID3D11Device*, WCHAR*);
	void Shutdown();

	ID3D11ShaderResourceView* GetTexture();

private:
	ID3D11ShaderResourceView* m_texture;
};


#endif