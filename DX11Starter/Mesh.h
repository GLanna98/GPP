#pragma once
#include <d3d11.h>

#include "Vertex.h"
class Mesh
{
private:
	//Buffer pointers
	ID3D11Buffer* vertexBuffer = nullptr;
	ID3D11Buffer* indexBuffer = nullptr;

	//Integer specifying how many indices are in the mesh's index buffer
	int meshIndices;

public:
	//Constructor
	Mesh(Vertex* vertices, int vertexCount, int* indices, int indexCount, ID3D11Device* bufferCreator);

	//Destructor
	virtual ~Mesh();

	ID3D11Buffer* GetVertexBuffer();
	ID3D11Buffer* GetIndexBuffer();
	int GetIndexCount();
};

