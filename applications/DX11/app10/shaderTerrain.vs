
cbuffer UniformBuffer {
	float4x4 projection;
	float4x4 view;
	float4x4 transform;
};

struct VertexInput {
	float3 position : POSITION;
	float2 texCoord : TEXCOORD;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 bitangent : BITANGENT;
};

struct PixelInput {
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD;
	float3 normal : NORMAL;
	float3x3 TBN : TBNMATRIX;
};

PixelInput main(VertexInput input) {
	PixelInput output;

	output.position = float4(input.position, 1.0);
	output.position = mul(output.position, transform);
	output.position = mul(output.position, view);
	output.position = mul(output.position, projection);

	output.texCoord = input.texCoord * 1024;

	output.normal = input.normal;

	// Create the TBN Matrix
	output.TBN = float3x3(input.tangent, input.bitangent, input.normal);
	output.TBN = mul(output.TBN, (float3x3)transform);

	return output;
}