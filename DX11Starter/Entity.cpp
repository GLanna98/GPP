#include "Entity.h"

//Constructor
Entity::Entity(Mesh* meshPtr)
{
	object = meshPtr;
	XMStoreFloat4x4(&worldMatrix, DirectX::XMMatrixIdentity());
	position = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	scale = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
	rotation = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
}

//Accessors
DirectX::XMFLOAT4X4 Entity::GetWorldMatrix() { return worldMatrix; }
void Entity::SetWorldMatrix(DirectX::XMFLOAT4X4 wM) { worldMatrix = wM; }

DirectX::XMFLOAT3 Entity::GetPosition() { return position; }
void Entity::SetPostion(DirectX::XMFLOAT3 pos) {	position = pos; }

DirectX::XMFLOAT3 Entity::GetScale() { return scale; }
void Entity::SetScale(DirectX::XMFLOAT3 scl) { scale = scl; }

DirectX::XMFLOAT3 Entity::GetRotation() { return rotation; }
void Entity::SetRotation(DirectX::XMFLOAT3 rot) { rotation = rot; }

void Entity::Move(float x, float y, float z)
{
	DirectX::XMFLOAT3 currentPos = position;
	position = DirectX::XMFLOAT3(currentPos.x + x, currentPos.y + y, currentPos.z + z);
}

Entity::~Entity()
{
}
