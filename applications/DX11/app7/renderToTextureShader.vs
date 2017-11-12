

cbuffer UniformBuffer {
	float4x4 projection;
	float4x4 view;
	float4x4 transform;
	float4 clipPlane;
	float alpha;
	float3 padding;
};

struct VertexInput {
	float3 position : POSITION;
	float2 texCoord : TEXCOORD;
};

struct PixelInput {
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD;

	float clip : SV_ClipDistance0;

	float alpha : ALPHA;
};

PixelInput main(VertexInput input){
	PixelInput output;

	output.position = float4(input.position, 1.0);
	output.position = mul(output.position, transform);
	output.position = mul(output.position, view);
	output.position = mul(output.position, projection);
	output.texCoord = input.texCoord;

	output.clip = dot(mul(float4(input.position, 1.0), transform), clipPlane);

	output.alpha = alpha;

	return output;
}