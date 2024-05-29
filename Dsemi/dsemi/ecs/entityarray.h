#ifndef ECS_ENTITY_POOL_H
#define ECS_ENTITY_POOL_H

#include <queue>
#include "Dsemi/ecs/componentflag.h"

// number of new entities to support each time memory gets reallocated
#define ECS_ENTITY_ARRAY_GROW_STRIDE 4096

namespace dsemi {
	namespace ecs {
		class entity_array {
		public:
			entity_array(size_t num_components);
			//~entity_array();

			size_t create_entity();

			void set_component(size_t entity_id, size_t component_id, bool value);
			bool get_component(size_t entity_id, size_t component_id);
			component_flag get_components(size_t entity_id);

			component_flag operator[](size_t index);

		private:
			std::vector<uint64_t> _entity_bytes;
			std::queue<size_t> _free_id_queue;
			size_t _next_id;
			size_t _entity_size;
			size_t _num_chunks;
		};
	}
}

#endif