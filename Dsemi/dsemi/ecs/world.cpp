#include "dsemi/ecs/world.h"
#include "dsemi/util/assert.h"

namespace dsemi {
	namespace ecs {
		world::world()
			: _entity_array(ECS_NUM_COMPONENTS)
		{
			_component_arrays = std::vector<component_array>(ECS_NUM_COMPONENTS);
		}

		entity_handle world::create_entity()
		{
			return entity_handle(_entity_array.create_entity(), this);
		}

		component_array& world::get_component_array(size_t component_id)
		{
			ASSERT(component_id < _component_arrays.size() && _component_arrays[component_id].is_valid(), "Attempting to access invalid component array");

			return _component_arrays[component_id];
		}
	}
}
