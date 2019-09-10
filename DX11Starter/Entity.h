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
};

