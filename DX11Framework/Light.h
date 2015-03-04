#pragma once
#include <DirectXMath.h>
class Light{
public:
	Light();
	Light(const Light&);
	~Light();

	void SetDiffuseColor(float r, float g, float b, float a);
	void SetDirection(float x, float y, float z);

	DirectX::XMFLOAT4 GetDiffuseColor();
	DirectX::XMFLOAT3 GetDirection();

private:
	DirectX::XMFLOAT4 m_diffuseColor;
	DirectX::XMFLOAT3 m_direction;

};