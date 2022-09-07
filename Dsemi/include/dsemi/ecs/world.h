#ifndef HEADER_DSEMI_ECS_WORLD
#define HEADER_DSEMI_ECS_WORLD

#include "dsemi/ecs/entityarray.h"
#include "dsemi/ecs/componentarray.h"

namespace dsemi {
	namespace ecs {
		class entity_handle;

		class world {
		public:
			world();

			entity_handle create_entity();

			template<typename T>
			T* add_component(size_t entity_id);

			template<typename T>
			T* get_component(size_t entity_id); // TODO: finish components of ecs (? what are my comments talking about)

			component_array& get_component_array(size_t component_id);

		private:
			entity_array _entity_array;
			std::vector<component_array> _component_arrays;

		};

		class entity_handle {
			friend class world;
			entity_handle(size_t id, world* world)
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

		public:
			class query
			{
			public:


			protected:
				query()
				{}
				~query()
				{}

			private:

			};

		private:
			const size_t _id;
			world* const _world;
		};

		/*==========================
		// template implementation
		============================*/
		template<typename T>
		T* world::add_component(size_t entity_id)
		{
			if (!_component_arrays[T::ID].is_valid())
			{
				_component_arrays[T::ID] = component_array(sizeof(T), T::ID);
			}

			_entity_array.set_component(entity_id, T::ID, true);
			_component_arrays[T::ID].insert(entity_id, new T());
			return (T*)_component_arrays[T::ID].at(entity_id);
		}

		template<typename T>
		T* world::get_component(size_t entity_id)
		{
			/*if (T::ID > _component_arrays.capacity())
				_component_arrays.resize(ECS_NUM_COMPONENTS);*/
			return (T*)_component_arrays[T::ID].at(entity_id);
		}
	}
}

#endif