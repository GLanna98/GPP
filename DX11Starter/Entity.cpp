#include "Entity.h"

//Constructor
Entity::Entity(Mesh* meshPtr, Material* matPtr)
{
	mesh = meshPtr;
	material = matPtr;
	DirectX::XMStoreFloat4x4(&worldMatrix, DirectX::XMMatrixIdentity());
	position = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	scale = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
	rotation = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
}

//Accessors
DirectX::XMFLOAT3 Entity::GetPosition() { return position; }
void Entity::SetPostion(DirectX::XMFLOAT3 pos) {	position = pos; }

DirectX::XMFLOAT3 Entity::GetScale() { return scale; }
void Entity::SetScale(DirectX::XMFLOAT3 scl) { scale = scl; }

DirectX::XMFLOAT3 Entity::GetRotation() { return rotation; }
void Entity::SetRotation(DirectX::XMFLOAT3 rot) { rotation = rot; }

Mesh* Entity::GetMesh() { return mesh; }

Material* Entity::GetMaterial() { return material; }

void Entity::PrepareMaterial(DirectX::XMFLOAT4X4 viewMatrix, DirectX::XMFLOAT4X4 projectionMatrix)
{
	worldMatrix = GetWorldMatrix();

	material->GetVertexShader()->SetMatrix4x4("world", worldMatrix);
	material->GetVertexShader()->SetMatrix4x4("view", viewMatrix);
	material->GetVertexShader()->SetMatrix4x4("projection", projectionMatrix);

	material->GetPixelShader()->SetShader();
	material->GetPixelShader()->CopyAllBufferData();

	material->GetVertexShader()->SetShader();
	material->GetVertexShader()->CopyAllBufferData();
}

DirectX::XMFLOAT4X4 Entity::GetWorldMatrix() 
{ 
	DirectX::XMMATRIX trans = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
	DirectX::XMMATRIX rot = DirectX::XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
	DirectX::XMMATRIX scl = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);

	DirectX::XMMATRIX world = scl * rot * trans;
	XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(world));

	return worldMatrix; 
}


void Entity::Move(float x, float y, float z)
{
	DirectX::XMFLOAT3 currentPos = position;
	position = DirectX::XMFLOAT3(currentPos.x + x, currentPos.y + y, currentPos.z + z);
}

Entity::~Entity()
{
}
