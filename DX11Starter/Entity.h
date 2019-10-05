#pragma once
#include <d3d11.h>
#include <DirectXMath.h>

#include "Mesh.h"
#include "Material.h"

class Entity
{
private:
	DirectX::XMFLOAT4X4 worldMatrix;
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 scale;
	DirectX::XMFLOAT3 rotation;
	Mesh* mesh;
	Material* material;
public:
	//Constructor
	Entity(Mesh* meshPtr, Material* matPtr);

	//Accessors
	DirectX::XMFLOAT4X4 GetWorldMatrix();
	DirectX::XMFLOAT3 GetPosition();
	void SetPostion(DirectX::XMFLOAT3 pos);
	DirectX::XMFLOAT3 GetScale();
	void SetScale(DirectX::XMFLOAT3 scl);
	DirectX::XMFLOAT3 GetRotation();
	void SetRotation(DirectX::XMFLOAT3 rot);

	Mesh* GetMesh();

	Material* GetMaterial();
	void PrepareMaterial(DirectX::XMFLOAT4X4 viewMatrix, DirectX::XMFLOAT4X4 projectionMatrix);

	//Method to move entity
	void Move(float x, float y, float z);

	void UpdateWorldMatrix();

	//Destructor
	virtual ~Entity();
};