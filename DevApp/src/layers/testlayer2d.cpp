#include "testlayer2d.h"

#include <iostream>
#include <dsemi/util/logger.hpp>

void test_layer_2dgfx::on_attach()
{
}

void test_layer_2dgfx::on_update(const float dt)
{
	//dsemi::logger::debug("[TestLayer2DGFX]: Update call");
}

void test_layer_2dgfx::on_event(dsemi::ievent& e)
{
	dsemi::event_dispatcher dispatcher(e);
	dispatcher.dispatch<dsemi::mouse_press_event>(BIND_EVENT(test_layer_2dgfx::_on_mouse_press));
}

void test_layer_2dgfx::on_render(const float dt)
{
	//dsemi::logger::debug("[TestLayer2DGFX]: Render call");
}

void test_layer_2dgfx::_on_create()
{
}

bool test_layer_2dgfx::_on_mouse_press(dsemi::mouse_press_event& e)
{
	dsemi::logger::debug("[TestLayer2DGFX]: mouse_press_event successfully handled and consumed.");
	return true;
}