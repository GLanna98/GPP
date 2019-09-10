#include "Entity.h"

//Constructor
Entity::Entity(Mesh* meshPtr)
{
	object = meshPtr;
	XMStoreFloat4x4(&worldMatrix, XMMatrixIdentity());
	position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
	rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
}

//Accessors
XMFLOAT4X4 Entity::GetWorldMatrix() { return worldMatrix; }
void Entity::SetWorldMatrix(XMFLOAT4X4 wM) { worldMatrix = wM; }

XMFLOAT3 Entity::GetPosition() { return position; }
void Entity::SetPostion(XMFLOAT3 pos) {	position = pos; }

XMFLOAT3 Entity::GetScale() { return scale; }
void Entity::SetScale(XMFLOAT3 scl) { scale = scl; }

XMFLOAT3 Entity::GetRotation() { return rotation; }
void Entity::SetRotation(XMFLOAT3 rot) { rotation = rot; }


