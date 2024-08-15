cbuffer cbuf : register(b0)
{
    row_major matrix projectionMatrix;
}
cbuffer cbuf2 : register(b1)
{
    float3 worldTransform;
}

struct vs_out {
	float4 position : SV_Position;
};

vs_out main(int2 pos : Position)
{
	vs_out output_vertex;

	// scale to pixel-based positions
	output_vertex.position = mul(float4(pos.x+worldTransform.x, pos.y+worldTransform.y, 1.0f+worldTransform.z, 1.0f), projectionMatrix);

	return output_vertex;
}
