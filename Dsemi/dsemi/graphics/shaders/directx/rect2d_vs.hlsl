struct VS_INPUT {
	float2 position : Position;
	float2 normal : Normal;
	float2 uv : TexCoord;
};

struct vs_out {
	float4 position : SV_Position;
	float4 colour : Colour;
};

vs_out main(VS_INPUT input)
{
	vs_out output_vertex;

	output_vertex.position = float4(input.position, 0.0f, 1.0f);

	output_vertex.colour = float4(
		1.0f,
		1.0f,
		1.0f,
		1.0f
		);

	return output_vertex;
}