
cbuffer UniformBuffer {
	float4x4 projection;
	float4x4 view;
	float4x4 transform;
	float4 clipPlane;
};

struct VertexInput {
	float3 position : POSITION;
	float2 texCoord : TEXCOORD;
	float3 normal : NORMAL;
};

struct PixelInput {
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD;
	float clip : SV_ClipDistance0;
};

PixelInput main(VertexInput input){
	PixelInput output;

	float4 position = float4(input.position, 1.0);
	float4 worldPos = mul(position, transform);

	output.position = mul(worldPos, view);
	output.position = mul(output.position, projection);

	output.texCoord = input.texCoord;

	output.clip = dot(worldPos, clipPlane);
	//output.clip = 1;

	return output;
}