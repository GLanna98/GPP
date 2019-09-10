#pragma once
#include <d3d11.h>
#include <DirectXMath.h>

#include "Mesh.h"

using namespace DirectX;

class Entity
{
private:
	XMFLOAT4X4 worldMatrix;
	XMFLOAT3 position;
	XMFLOAT3 scale;
	XMFLOAT3 rotation;
	Mesh* object;
public:
	//Constructor
	Entity(Mesh* meshPtr);

	//Accessors
	XMFLOAT4X4 GetWorldMatrix();
	void SetWorldMatrix(XMFLOAT4X4 wM);
	XMFLOAT3 GetPosition();
	void SetPostion(XMFLOAT3 pos);
	XMFLOAT3 GetScale();
	void SetScale(XMFLOAT3 scl);
	XMFLOAT3 GetRotation();
	void SetRotation(XMFLOAT3 rot);

	void Move(float distance, float direction);
};

