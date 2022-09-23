#include "testlayercore.h"
#include <dsemi/util/logger.hpp>
#include <iostream>

void test_layer_core::on_update(const float dt)
{
}

void test_layer_core::on_event(dsemi::ievent& e)
{
	dsemi::event_dispatcher dispatcher(e);
	dispatcher.dispatch<dsemi::window_focus_event>(BIND_EVENT(test_layer_core::_on_mouse_press));
	//dispatcher.dispatch<dsemi::mouse_scroll_event>(BIND_EVENT(test_layer_core::_on_mouse_scroll));
}

bool test_layer_core::_on_mouse_press(dsemi::window_focus_event& e)
{
	dsemi::logger::debug("[TestLayerCore]: gained focus successfully handled and consumed.");
	return true;
}