
struct VertexInput{
	float3 position : POSITION;
};

struct PixelInput{
	float4 position : SV_POSITION;
	float3 color : COLOR;
};

PixelInput main(VertexInput input){
	PixelInput output;
	float4 position;
	position = float4(input.position, 1.0);
	output.position = position;
	output.color = float4(0,1,0,1);

	return output;
}