#pragma once
#include <d3d11.h>
#include <DirectXMath.h>

#include "SimpleShader.h"

class Material
{
private:
	SimplePixelShader* pixelShader = nullptr;
	SimpleVertexShader* vertexShader = nullptr;
public:
	Material(SimplePixelShader* pShader, SimpleVertexShader* vShader);

	SimplePixelShader* GetPixelShader();
	SimpleVertexShader* GetVertexShader();
};

