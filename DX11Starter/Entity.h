#pragma once
#include <d3d11.h>
#include <DirectXMath.h>

#include "Mesh.h"

class Entity
{
private:
	DirectX::XMFLOAT4X4 worldMatrix;
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 scale;
	DirectX::XMFLOAT3 rotation;
	Mesh* object;
public:
	//Constructor
	Entity(Mesh* meshPtr);

	//Accessors
	DirectX::XMFLOAT4X4 GetWorldMatrix();
	void SetWorldMatrix(DirectX::XMFLOAT4X4 wM);
	DirectX::XMFLOAT3 GetPosition();
	void SetPostion(DirectX::XMFLOAT3 pos);
	DirectX::XMFLOAT3 GetScale();
	void SetScale(DirectX::XMFLOAT3 scl);
	DirectX::XMFLOAT3 GetRotation();
	void SetRotation(DirectX::XMFLOAT3 rot);

	//Method to move entity
	void Move(float x, float y, float z);

	//Destructor
	virtual ~Entity();
};