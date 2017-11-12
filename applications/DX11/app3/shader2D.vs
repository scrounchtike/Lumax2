
struct VertexInput{
	float2 position : POSITION;
};

struct PixelInput{
	float4 position : SV_POSITION;
};

PixelInput main(VertexInput input){
	PixelInput output;

	output.position = float4(input.position, 0.0, 1.0);

	return output;
}