#include "testlayercore.h"
#include <dsemi/util/logger.hpp>


void test_layer_core::on_update(const float dt)
{
}

void test_layer_core::on_render(const float dt)
{
}

void test_layer_core::on_event(dsemi::ievent& e)
{
	dsemi::event_dispatcher dispatcher(e);
	//std::cout << "[TestLayerCore] New event: " << e.to_string() << std::endl;
	dispatcher.dispatch<dsemi::mouse_press_event>(BIND_EVENT(test_layer_core::_on_mouse_press));
}

bool test_layer_core::_on_mouse_press(dsemi::mouse_press_event& e)
{
	dsemi::logger::debug("[TestLayerCore]: mouse_press_event successfully handled and consumed.");
	return true;
}