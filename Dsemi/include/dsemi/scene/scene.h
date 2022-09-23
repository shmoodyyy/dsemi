#ifndef HEADER_DSEMI_CORE_SCENE
#define HEADER_DSEMI_CORE_SCENE

#include "dsemi/core/layerstack.h"
#include "dsemi/ecs/world.h"

namespace dsemi {
	class scene {
	public:
		scene();
		~scene();

		void handle_update(const float dt);
		void handle_event (ievent& e);

		ecs::entity_handle add_entity();
		template<typename T>
		void add_component(ecs::entity_handle& entity);

	protected:
		layer_stack _layer_stack;
		ecs::world  _entities;
	};
}

#endif