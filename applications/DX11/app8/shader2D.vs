
cbuffer UniformBuffer {
	float4x4 transform;
};

struct VertexInput {
	float2 position : POSITION;
	float2 texCoord : TEXCOORD;
};

struct PixelInput {
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD;
};

PixelInput main(VertexInput input){
	PixelInput output;

	output.position = float4(input.position, 0.0, 1.0);
	output.position = mul(output.position, transform);
	output.texCoord = input.texCoord;
	output.texCoord.y = 1.0 - output.texCoord.y;

	return output;
}