#include "Material.h"

Material::Material(SimplePixelShader* pShader, SimpleVertexShader* vShader)
{
	pixelShader = pShader;
	vertexShader = vShader;
}

SimplePixelShader* Material::GetPixelShader() { return pixelShader; }

SimpleVertexShader* Material::GetVertexShader() { return vertexShader; }
