#ifndef _CAMERACLASS_H_
#define _CAMERACLASS_H_

#include <DirectXMath.h>

__declspec(align(16)) class Camera{
public:
	Camera();
	Camera(const Camera&);
	~Camera();
	
	void SetPos(float, float, float);
	void SetRot(float, float, float);
	DirectX::XMFLOAT3 GetPos();
	DirectX::XMFLOAT3 GetRot();

	void Render();
	void GetViewMatrix(DirectX::XMMATRIX&);

	void* operator new(size_t i){
		return _mm_malloc(i, 16);
	}
	void operator delete(void* p){
		_mm_free(p);
	}
private:
	float m_posX, m_posY, m_posZ;
	float m_rotX, m_rotY, m_rotZ;
	DirectX::XMMATRIX m_viewMatrix;
};

#endif