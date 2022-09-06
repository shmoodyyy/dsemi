#include "dspch.h"
#include "dsemi/core/scene.h"

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

	void scene::handle_render(const float dt)
	{
		_layer_stack.call_render(dt);
	}
}