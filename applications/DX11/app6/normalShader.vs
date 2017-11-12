
cbuffer UniformBuffer {
	float4x4 projection;
	float4x4 view;
	float4x4 transform;
	float3 cameraPos;
	float padding;
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
	float3x3 TBN : TBNMATRIX;
	float3 cameraPos : CAMERA;
};

PixelInput main(VertexInput input){
	PixelInput output;

	float4x4 modelView = transform;
	modelView = mul(modelView, view);

	output.position = float4(input.position, 1.0);
	output.position = mul(output.position, modelView);
	output.position = mul(output.position, projection);
	output.texCoord = input.texCoord;

	output.TBN = float3x3(input.tangent, input.bitangent, input.normal);
	output.TBN = mul(output.TBN, (float3x3)transform);

	output.cameraPos = cameraPos;

	return output;
}