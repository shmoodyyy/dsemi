cbuffer view_buf : register(b0)
{
	float view_width;
	float view_height;
}

struct vs_out {
	float4 position : SV_Position;
};

vs_out main(int2 pos : Position)
{
	vs_out output_vertex;

	// scale to pixel-based positions
	output_vertex.position =
		float4(
			pos.x * 2.0f / view_width,
			pos.y * 2.0f / view_height,
			1.0f, 1.0f
	);

	return output_vertex;
}