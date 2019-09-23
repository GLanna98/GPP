#include "Camera.h"

DirectX::XMFLOAT4X4 Camera::GetViewMatrix() { return viewMatrix; }
DirectX::XMFLOAT4X4 Camera::GetProjectionMatrix() { return projectionMatrix; }

Camera::Camera()
{
	cameraDirection = forward;
	cameraPosition = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);

	xRotation = 0.0f;
	yRotation = 0.0f;
}

void Camera::Update()
{	
	DirectX::XMStoreFloat3(&cameraDirection, DirectX::XMVector3Rotate(DirectX::XMLoadFloat3(&forward), DirectX::XMQuaternionRotationRollPitchYaw(xRotation, yRotation, 0.0f)));

	DirectX::XMMATRIX newViewMatrix = DirectX::XMMatrixLookToLH(DirectX::XMLoadFloat3(&cameraPosition), DirectX::XMLoadFloat3(&cameraDirection), DirectX::XMLoadFloat3(&up));

	DirectX::XMStoreFloat4x4(&viewMatrix, newViewMatrix);

	DirectX::XMVECTOR sideVector = DirectX::XMVector3Cross(DirectX::XMLoadFloat3(&cameraDirection), DirectX::XMLoadFloat3(&up));

	//Camera Movement
	if (GetAsyncKeyState('W') & 0x8000) 
	{
		DirectX::XMStoreFloat3(&cameraPosition, DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&cameraPosition), DirectX::XMLoadFloat3(&cameraDirection)));
	}
	if (GetAsyncKeyState('S') & 0x8000)
	{
		DirectX::XMStoreFloat3(&cameraPosition, DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&cameraPosition), DirectX::XMLoadFloat3(&cameraDirection)));
	}
	if (GetAsyncKeyState('D') & 0x8000)
	{
		DirectX::XMStoreFloat3(&cameraPosition, DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&cameraPosition), sideVector));
	}
	if (GetAsyncKeyState('A') & 0x8000)
	{
		DirectX::XMStoreFloat3(&cameraPosition, DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&cameraPosition), sideVector));
	}
	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		cameraPosition.y += 1;
	}
	if (GetAsyncKeyState('X') & 0x8000)
	{
		cameraPosition.y -= 1;
	}
}

void Camera::UpdateProjectionMatrix(float aspectRatio)
{
	DirectX::XMMATRIX P = DirectX::XMMatrixPerspectiveFovLH(
		0.25f * 3.1415926535f,	// Field of View Angle
		aspectRatio,			// Aspect ratio
		0.1f,				  	// Near clip plane distance
		100.0f);			  	// Far clip plane distance
	DirectX::XMStoreFloat4x4(&projectionMatrix, DirectX::XMMatrixTranspose(P)); // Transpose for HLSL!
}

void Camera::RotateCamera(int XpixelAmount, int YpixelAmount)
{
	xRotation += (float)XpixelAmount * (DirectX::XM_PI / 180.0f);
	xRotation = fmod(xRotation, DirectX::XM_2PI);

	yRotation += (float)YpixelAmount * (DirectX::XM_PI / 180.0f);
	yRotation = fmod(yRotation, DirectX::XM_2PI);
}
