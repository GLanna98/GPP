#include "Camera.h"

DirectX::XMFLOAT4X4 Camera::GetViewMatrix() { return viewMatrix; }
DirectX::XMFLOAT4X4 Camera::GetProjectionMatrix() { return projectionMatrix; }

Camera::Camera()
{
	DirectX::XMFLOAT4X4 viewMatrix;
	DirectX::XMFLOAT4X4 projectionMatrix;

	DirectX::XMFLOAT3 cameraPosition = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	DirectX::XMFLOAT3 cameraDirection;

	float xRotation = 0.0f;
	float yRotation = 0.0f;
}

void Camera::Update()
{	
	DirectX::XMStoreFloat3(&cameraDirection, DirectX::XMVector3Rotate(DirectX::XMLoadFloat3(&forward), DirectX::XMQuaternionRotationRollPitchYaw(xRotation, yRotation, 0.0f)));

	DirectX::XMStoreFloat4x4(&viewMatrix, DirectX::XMMatrixLookToLH(DirectX::XMLoadFloat3(&cameraPosition), DirectX::XMLoadFloat3(&cameraDirection), DirectX::XMLoadFloat3(&up)));

	//Camera Movement
	if (GetAsyncKeyState('W') & 0x8000) 
	{
		DirectX::XMStoreFloat3(&cameraPosition, DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&cameraPosition), DirectX::XMLoadFloat3(&cameraDirection)));
	}
	if (GetAsyncKeyState('W') & 0x8000)
	{
		DirectX::XMStoreFloat3(&cameraPosition, DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&cameraPosition), DirectX::XMLoadFloat3(&cameraDirection)));
	}
	if (GetAsyncKeyState('W') & 0x8000)
	{
		DirectX::XMStoreFloat3(&cameraPosition, DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&cameraPosition), DirectX::XMLoadFloat3(&cameraDirection)));
	}
	if (GetAsyncKeyState('W') & 0x8000)
	{
		DirectX::XMStoreFloat3(&cameraPosition, DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&cameraPosition), DirectX::XMLoadFloat3(&cameraDirection)));
	}
}

void Camera::UpdateProjectionMatrix()
{
}
