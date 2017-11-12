
cbuffer UniformBuffer {
	float4x4 transform;
	float xPos;
	float yPos;
	float width;
	float height;
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

	output.texCoord.x = xPos + input.position.x * width;
	output.texCoord.y = yPos - input.position.y * height;

	return output;
}