#pragma once
#include <d3d11.h>
#include <DirectXMath.h>

#include "SimpleShader.h"

class Material
{
private:
	SimplePixelShader* pixelShader = nullptr;
	SimpleVertexShader* vertexShader = nullptr;
	ID3D11ShaderResourceView* resourceView = nullptr;
	ID3D11SamplerState* samplerState = nullptr;
public:
	Material(SimplePixelShader* pShader, SimpleVertexShader* vShader, ID3D11ShaderResourceView* resourceViewPtr, ID3D11SamplerState* samplerStatePtr);

	SimplePixelShader* GetPixelShader();
	SimpleVertexShader* GetVertexShader();
	ID3D11ShaderResourceView* GetResourceView();
	ID3D11SamplerState* GetSamplerState();
};

