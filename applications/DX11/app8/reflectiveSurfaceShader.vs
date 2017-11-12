
cbuffer UniformBuffer {
	float4x4 projection;
	float4x4 view;
	float4x4 transform;
};

struct VertexInput {
	float3 position : POSITION;
	float2 texCoord : TEXCOORD;
	float3 normal : NORMAL;
};

struct PixelInput {
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD;
	float3 normal : NORMAL;
};

PixelInput main(VertexInput input){
	PixelInput output;

	output.position = float4(input.position, 1.0);
	output.position = mul(output.position, transform);
	output.position = mul(output.position, view);
	output.position = mul(output.position, projection);

	output.texCoord = input.texCoord;

	// TODO: Transform the normal accordingly with the transform matrix
	output.normal = input.normal;

	return output;
}