#include "dspch.h"
#include "dsemi/scene/scene.h"

namespace dsemi {
	scene::scene()
	{
	}

    scene::~scene()
    {
    }

	void scene::handle_update(const float dt)
	{
		_layer_stack.call_update(dt);
	}

	void scene::handle_event(ievent& e)
	{
		_layer_stack.send_event(e);
	}

	ecs::entity_handle scene::add_entity()
	{
		return _entities.create_entity();
	}

	template<typename T>
	void scene::add_component(ecs::entity_handle& entity)
	{
		_entities.add_component<T>(entity);
	}
}