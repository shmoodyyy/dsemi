struct ps_in {
	float4 position : SV_Position;
};

float4 main(ps_in input) : SV_TARGET
{
	return float4(0.8f, 0.3f, 1.0f, 1.0f);
}
