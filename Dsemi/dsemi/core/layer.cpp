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

	void layer::handle_event(ievent& e)
	{
		on_event(e);
	}
}