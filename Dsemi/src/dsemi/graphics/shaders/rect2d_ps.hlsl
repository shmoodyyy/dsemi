struct ps_in {
	float4 position : SV_Position;
	float4 colour : Colour;
};

float4 main(ps_in input) : SV_TARGET
{
	return input.colour;
}