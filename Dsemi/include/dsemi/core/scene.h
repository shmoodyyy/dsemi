#ifndef HEADER_DSEMI_CORE_SCENE
#define HEADER_DSEMI_CORE_SCENE

#include "dsemi/core/layerstack.h"

namespace dsemi {
	class scene {
	public:
		scene();
		~scene();

		virtual void on_load() {}
		virtual void on_unload() {}

		void handle_update(const float dt);
		void handle_event(ievent& e);
		void handle_render(const float dt);

	protected:
		layer_stack _layer_stack;
	};
}

#endif