#ifndef DSEMI_LAYER_H
#define DSEMI_LAYER_H

#include "Dsemi/Events/Event.h"

namespace dsemi {

	/* Base class for Application Layers
	virtual functions:
		-- void on_attach()
		-- void on_detach()
		-- void on_update(const float)
		-- void on_event(IEvent&)
		-- void on_render(const float)
	*/
	class layer
	{
	public:
		layer(const std::string& label = "Layer");
		virtual ~layer();

		virtual void on_attach() {}
		virtual void on_detach() {}
		virtual void on_update(const float dt) {}
		virtual void on_event(ievent& e) {}
		virtual void on_render(const float dt) {}

		void update(const float dt);
		void handle_event(ievent& e);
		void render(const float dt);

		inline void set_visible(bool val) { _render_active = val; }
		inline bool is_visible() { return _render_active; }

		inline void set_active(bool val) { _update_active = val; }
		inline bool is_active() { return _update_active; }

		inline const std::string& get_label() const { return _label; }

	protected:
		bool _render_active = true;
		bool _update_active = true;
		std::string _label;
	};
}

#endif