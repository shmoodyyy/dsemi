#include "dspch.h"
#include "Dsemi/ecs/world.h"

namespace dsemi {

	ecs_world::ecs_world()
		: _entity_array(ECS_NUM_COMPONENTS)
	{
		_component_arrays = std::vector<ecs_component_array>(ECS_NUM_COMPONENTS);
	}

	ecs_entity_handle ecs_world::create_entity()
	{
		return ecs_entity_handle(_entity_array.create_entity(), this);
	}

	ecs_component_array& ecs_world::get_component_array(size_t component_id)
	{
		ASSERT(component_id < _component_arrays.size() && _component_arrays[component_id].is_valid(), "Attempting to access invalid component array");

		return _component_arrays[component_id];
	}
}