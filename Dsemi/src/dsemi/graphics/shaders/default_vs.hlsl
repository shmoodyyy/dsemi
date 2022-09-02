struct vs_in {
	uint id : SV_VertexID;
	float2 position : Position;
};

struct vs_out {
	float4 position : SV_Position;
	float4 colour : Colour;
};

vs_out main(vs_in input_vertex)
{
	vs_out output_vertex;

	output_vertex.position = float4(input_vertex.position, 0.0f, 1.0f);

	output_vertex.colour = float4(
		1.0f * (input_vertex.id % 3 == 0),
		1.0f * (input_vertex.id % 3 == 1),
		1.0f * (input_vertex.id % 3 == 2),
		1.0f
		);

	return output_vertex;
}