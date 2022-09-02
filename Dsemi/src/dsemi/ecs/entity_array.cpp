#include "dspch.h"
#include "Dsemi/ecs/entity_array.h"

namespace dsemi {

	ecs_entity_array::ecs_entity_array(size_t num_components)
		: _next_id(0), 
		  _entity_size(num_components / 64u + (num_components % 64u > 0))
	{
		_entity_bytes.reserve(ECS_ENTITY_ARRAY_GROW_STRIDE * _entity_size);
		_num_chunks = 1u;
		for (auto& byte : _entity_bytes)
			byte = 0;
	}

	size_t ecs_entity_array::create_entity()
	{
		if (_free_id_queue.empty())
		{
			if (_next_id / ECS_ENTITY_ARRAY_GROW_STRIDE >= _num_chunks)
			{
				_num_chunks++;
				_entity_bytes.reserve(ECS_ENTITY_ARRAY_GROW_STRIDE * _entity_size * _num_chunks);
			}
			for (size_t i = 0; i < _entity_size; ++i)
			{
				_entity_bytes.emplace_back(0);
			}
			return _next_id++;
		}
		else
		{
			size_t new_id = _free_id_queue.front();
			_free_id_queue.pop();
			return new_id;
		}
	}

	void ecs_entity_array::set_component(size_t entity_id, size_t component_id, bool value)
	{
		if (entity_id * _entity_size > _entity_bytes.capacity())
			return;
		// TODO: handle dynamic resizing when new components get added
		// relevant for later when adding components via scripting (lua or c# or something)
		uint64_t& byte = _entity_bytes[entity_id * _entity_size + component_id / 64u];
		byte = byte & ~(1 << component_id) | ((uint64_t)value << component_id);
	}

	bool ecs_entity_array::get_component(size_t entity_id, size_t component_id)
	{
		if (entity_id * _entity_size > _entity_bytes.capacity())
			return false;

		uint64_t& byte = _entity_bytes[entity_id * _entity_size + component_id / 64u];
		return byte & (1 << component_id % 64u);
	}

	ecs_component_flag ecs_entity_array::get_components(size_t entity_id)
	{
		ecs_component_flag out(_entity_size * 64u);

		for (size_t i = 0; i < _entity_size; ++i)
		{
			out.set_element(i, _entity_bytes[i]);
		}

		return out;
	}

	ecs_component_flag ecs_entity_array::operator[](size_t entity_id)
	{
		if (entity_id * _entity_size > _entity_bytes.capacity())
			return ecs_component_flag(0);

		return get_components(entity_id);
	}
}