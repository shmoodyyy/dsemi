#include "app.h"

int main(int argc, const char** argv)
{
	using namespace dsemi::graphics;
	DevApp app;

	//VertexLayout layout = {
	//	{"Position", shader_data_type::FLOAT3},
	//	{"Normal", shader_data_type::FLOAT3},
	//	{"TexCoord", shader_data_type::FLOAT2},
	//};

	//vertex_array vertices(layout);
	//auto pos = { 5.0f, 5.0f, 5.0f };
	//auto nor = { 5.0f, 5.0f, 5.0f };
	//auto tex = { 5.0f, 5.0f};
	//vertices.emplace_back(
	//	pos,
	//	nor,
	//	tex
	//);

	app.init();
	app.run();

	return 0;
}
