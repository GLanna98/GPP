#include "Entity.h"

Entity::Entity(Mesh* meshPtr)
{
	object = meshPtr;
	XMStoreFloat4x4(&worldMatrix, XMMatrixIdentity());
	position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
	rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
}
