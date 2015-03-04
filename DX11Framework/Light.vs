cbuffer MatrixBuffer{
	matrix world;
	matrix view;
	matrix proj;
};

struct VertexInputType{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

struct PixelInputType{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

PixelInputType LightVertexShader(VertexInputType input){
	PixelInputType output;

	input.position.w = 1.0f;

	output.position = mul(input.position, world);
	output.position = mul(output.position, view);
	output.position = mul(output.position, proj);

	output.tex = input.tex;

	output.normal = mul(input.normal, (float3x3)world);

	output.normal = normalize(output.normal);


	return output;
}