#include "dspch.h"

#include "Dsemi/Events/Event.h"

#include "Dsemi/Core/Layer.h"

namespace dsemi {
	layer::layer(const std::string& label /* = "Layer" */)
		: _label(label)
	{
	}

	layer::~layer()
	{
	}

	void layer::update(const float dt)
	{
		if (!_update_active)
			return;

		on_update(dt);
		//EntitySystem.Update(dt);
	}

	// todo: i dont like the way this whole event thing works id much rather try using macros to bind events to event receivers
	void layer::handle_event(ievent& e)
	{
		on_event(e);
	}

	void layer::render(const float dt)
	{
		if (!_render_active)
			return;

		on_render(dt);

		//EntitySystem.Render();
	}
}