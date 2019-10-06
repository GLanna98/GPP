
// Struct representing the data we expect to receive from earlier pipeline stages
// - Should match the output of our corresponding vertex shader
// - The name of the struct itself is unimportant
// - The variable names don't have to match other shaders (just the semantics)
// - Each variable must have a semantic, which defines its usage
struct VertexToPixel
{
	// Data type
	//  |
	//  |   Name          Semantic
	//  |    |                |
	//  v    v                v
	float4 position		: SV_POSITION;
	/*float4 color		: COLOR;*/
	float3 normal		: NORMAL;
	float2 uv			: UV;
};

struct DirectionalLight
{
	float4 AmbientColor;
	float4 DiffuseColor;
	float3 Direction;
};

cbuffer externalDLight : register(b0)
{
	DirectionalLight light1;
	DirectionalLight light2;
};

// --------------------------------------------------------
// The entry point (main method) for our pixel shader
// 
// - Input is the data coming down the pipeline (defined by the struct)
// - Output is a single color (float4)
// - Has a special semantic (SV_TARGET), which means 
//    "put the output of this into the current render target"
// - Named "main" because that's the default the shader compiler looks for
// --------------------------------------------------------
float4 main(VertexToPixel input) : SV_TARGET
{
	float3 lightDir1 = normalize(-light1.Direction);
	float3 lightDir2 = normalize(-light2.Direction);
	input.normal = normalize(input.normal);
	float lightAmount1 = saturate(dot(input.normal, -lightDir1));
	float4 finalLight1 = light1.DiffuseColor * lightAmount1 + light1.AmbientColor;
	float lightAmount2 = saturate(dot(input.normal, -lightDir2));
	float4 finalLight2 = light2.DiffuseColor * lightAmount2 + light2.AmbientColor;

	return finalLight1 + finalLight2;
}