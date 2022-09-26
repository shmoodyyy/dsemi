#include <dsemi.h>
#include <iostream>
#include "scenes/testscene2d.h"
#include <dsemi/graphics/color.h>

class DevApp : public dsemi::application
{
public:
	DevApp()
	{
	}

	~DevApp()
	{
	}

private:
	virtual void _on_init() override
	{
		active_scene = &_test_scene;
		test.set_event_callback(BIND_EVENT(DevApp::handle_event));
		test.create(640, 480, L"Test 2");
	}

	virtual void on_event(dsemi::ievent& e) override
	{
		//test_layer_core.on_event(e);
	}

private:
	test_scene2d _test_scene;
	dsemi::window test;
};

int main()
{
	using namespace dsemi::graphics;
	DevApp app;

	vertex_layout layout = {
		{"Position", shader_data_type::FLOAT3},
		{"Normal", shader_data_type::FLOAT3},
		{"TexCoord", shader_data_type::FLOAT2},
	};

	vertex_array vertices(layout);
	auto pos = { 5.0f, 5.0f, 5.0f };
	auto nor = { 5.0f, 5.0f, 5.0f };
	auto tex = { 5.0f, 5.0f};
	vertices.emplace_back(
		pos,
		nor,
		tex
	);

	app.init();
	app.run();

	return 0;
}