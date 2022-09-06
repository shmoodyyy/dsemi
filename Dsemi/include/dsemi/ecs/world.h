#ifndef ECS_WORLD_H
#define ECS_WORLD_H

#include "Dsemi/ecs/entityarray.h"
#include "Dsemi/ecs/componentarray.h"

namespace dsemi {
	class ecs_entity_handle;

	class ecs_world {
	public:
		ecs_world();

		ecs_entity_handle create_entity();
		
		template<typename T>
		T* add_component(size_t entity_id);

		template<typename T>
		T* get_component(size_t entity_id); // TODO: finish components of ecs (? what are my comments talking about)

		ecs_component_array& get_component_array(size_t component_id);

	private:
		ecs_entity_array _entity_array;
		std::vector<ecs_component_array> _component_arrays;

	};

	class ecs_entity_handle {
		friend class ecs_world;
		ecs_entity_handle(size_t id, ecs_world* world)
			: _id(id), _world(world)
		{}

	public:

		template<typename T>
		T* add_component()
		{
			return _world->add_component<T>(_id);
		}

		template<typename T>
		T* get_component()
		{
			return _world->get_component<T>(_id);
		}

	private:
		const size_t _id;
		ecs_world* const _world;
	};

	/*==========================
	// template implementation
	============================*/
	template<typename T>
	T* ecs_world::add_component(size_t entity_id)
	{
		if (!_component_arrays[T::ID].is_valid())
		{
			_component_arrays[T::ID] = ecs_component_array(sizeof(T), T::ID);
		}

		_entity_array.set_component(entity_id, T::ID, true);
		_component_arrays[T::ID].insert(entity_id, new T());
		return (T*)_component_arrays[T::ID].at(entity_id);
	}

	template<typename T>
	T* ecs_world::get_component(size_t entity_id)
	{
		/*if (T::ID > _component_arrays.capacity())
			_component_arrays.resize(ECS_NUM_COMPONENTS);*/
		return (T*)_component_arrays[T::ID].at(entity_id);
	}
}

#endif