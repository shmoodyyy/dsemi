struct vs_out {
	float4 position : SV_Position;
};

vs_out main(int2 pos : Position)
{
	vs_out output_vertex;

	output_vertex.position = float4(pos, 1.0f, 1.0f);

	return output_vertex;
}