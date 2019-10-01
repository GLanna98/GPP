#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <Windows.h>

class Camera
{
private:
	DirectX::XMFLOAT4X4 viewMatrix;
	DirectX::XMFLOAT4X4 projectionMatrix;

	DirectX::XMFLOAT3 cameraPosition;
	DirectX::XMFLOAT3 cameraDirection;

	float xRotation;
	float yRotation;

	//Defaults
	DirectX::XMFLOAT3 right = DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f);
	DirectX::XMFLOAT3 up = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);
	DirectX::XMFLOAT3 forward = DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f);

public:
	DirectX::XMFLOAT4X4 GetViewMatrix();
	DirectX::XMFLOAT4X4 GetProjectionMatrix();
	DirectX::XMFLOAT3 GetPosition();

	Camera();

	void Update(float deltaTime, float totalTime);

	void UpdateProjectionMatrix(float aspectRatio);

	void RotateCamera(int XpixelAmount, int YpixelAmount);
};

