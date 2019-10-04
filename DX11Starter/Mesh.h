#pragma once
#include <d3d11.h>
#include <vector>
#include <fstream>

#include "Vertex.h"
class Mesh
{
private:
	//Buffer pointers
	ID3D11Buffer* vertexBuffer = nullptr;
	ID3D11Buffer* indexBuffer = nullptr;

	//Integer specifying how many indices are in the mesh's index buffer
	int meshIndices = 0;

	void CreateBuffers(Vertex* vertices, int vertexCount, UINT* indices, int indexCount, ID3D11Device* device);

public:
	//Constructor
	Mesh(Vertex* vertices, int vertexCount, UINT* indices, int indexCount, ID3D11Device* device);
	Mesh(char* fileName, ID3D11Device* device);

	//Destructor
	virtual ~Mesh();

	ID3D11Buffer* GetVertexBuffer();
	ID3D11Buffer* GetIndexBuffer();
	int GetIndexCount();
};

