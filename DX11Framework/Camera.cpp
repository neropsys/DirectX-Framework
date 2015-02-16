#include "Camera.h"

using namespace DirectX;

Camera::Camera():m_posX(0.f), m_posY(0.f), m_posZ(0.f), m_rotX(0.f), m_rotY(0.f), m_rotZ(0.f){
}

Camera::Camera(const Camera& other){
}
Camera::~Camera(){}

void Camera::SetPos(float x, float y, float z){
	m_posX = x;
	m_posY = y;
	m_posZ = z;
	return;
}
void Camera::SetRot(float x, float y, float z){
	m_rotX = x;
	m_rotY = y;
	m_rotZ = z;
	return;
}

XMFLOAT3 Camera::GetPos(){ return XMFLOAT3(m_posX, m_posY, m_posZ); }
XMFLOAT3 Camera::GetRot(){ return XMFLOAT3(m_rotX, m_rotY, m_rotZ); }

void Camera::Render(){
	XMFLOAT3 up, pos, lookAt;
	float yaw, pitch, roll;
	XMMATRIX rotMatrix;

	up.x = 0.f;
	up.y = 1.f;
	up.z = 0.f;

	pos.x = m_posX;
	pos.y = m_posY;
	pos.z = m_posZ;

	lookAt.x = 0.f;
	lookAt.y = 0.f;
	lookAt.z = 1.f;

	pitch = m_rotX * 0.0174532925f;
	yaw = m_rotY * 0.0174532925f;
	roll = m_rotZ * 0.0174532925f;


	rotMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	XMStoreFloat3(&lookAt, XMVector3TransformCoord(XMLoadFloat3(&lookAt), rotMatrix));
	XMStoreFloat3(&up, XMVector3TransformCoord(XMLoadFloat3(&up), rotMatrix));

	//modified header file. User must overload operator on DirectXMath header file
	lookAt = lookAt + pos;

	m_viewMatrix = XMMatrixLookAtLH(XMLoadFloat3(&pos), XMLoadFloat3(&lookAt), XMLoadFloat3(&up));
	return;
}

void Camera::GetViewMatrix(XMMATRIX& viewMatrix){
	viewMatrix = m_viewMatrix;
	return;
}

