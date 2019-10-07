#include "Material.h"

Material::Material(SimplePixelShader* pShader, SimpleVertexShader* vShader, ID3D11ShaderResourceView* resourceViewPtr, ID3D11SamplerState* samplerStatePtr)
{
	pixelShader = pShader;
	vertexShader = vShader;
	resourceView = resourceViewPtr;
	samplerState = samplerStatePtr;
}

SimplePixelShader* Material::GetPixelShader() { return pixelShader; }

SimpleVertexShader* Material::GetVertexShader() { return vertexShader; }

ID3D11ShaderResourceView* Material::GetResourceView(){ return resourceView; }

ID3D11SamplerState* Material::GetSamplerState(){ return samplerState; }
