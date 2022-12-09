#include "testlayer2d.h"

#include <iostream>
#include <dsemi/util/logger.hpp>
#include <dsemi/ecs/ecs.h>
#include "../ecs/test2dcomponents.h"

void test_layer_2dgfx::on_attach()
{
	dsemi::ecs::entity_handle new_ent = _entities->create_entity();
	new_ent.add_component<transform2d>();
	new_ent.add_component<sprite_render>();
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

void test_layer_2dgfx::_on_create()
{
}

bool test_layer_2dgfx::_on_mouse_press(dsemi::mouse_press_event& e)
{
	dsemi::logger::debug(L"[TestLayer2DGFX]: mouse_press_event successfully handled and consumed.");
	return true;
}