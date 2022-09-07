#include "dspch.h"
#include "dsemi/ecs/componentarray.h"

#include "dsemi/ecs/ecsdefines.h"

namespace dsemi {
	namespace ecs {
		component_array::component_array()
			: _allocated(nullptr), _element_size(0u), _assigned_id(ECS_INVALID_COMPONENT)
		{
		}

		component_array::component_array(
			size_t component_size,
			size_t component_id
		) : _element_size(component_size),
			_assigned_id(component_id)
		{
			// allocate required memory to only one 4096 block of elements at first
			size_t allocate_bytes = component_size * ECS_COMPONENT_ARRAY_GROW_STRIDE;
			_allocated = malloc(allocate_bytes);
			_capacity = ECS_COMPONENT_ARRAY_GROW_STRIDE;

			// initialize bytes to 0
			for (size_t i = 0; i < allocate_bytes; ++i)
			{
				*((char*)_allocated + i) = 0;
			}
		}

		component_array::component_array(const component_array& other)
			: _element_size(other._element_size), _assigned_id(other._assigned_id), _capacity(other._capacity)
		{
			_allocated = nullptr;
			if (!is_valid())
				return;

			_allocated = malloc(_element_size * _capacity);
			for (size_t i = 0; i < _capacity * _element_size; i++)
			{
				((char*)_allocated)[i] = ((char*)other._allocated)[i];
			}
		}

		component_array& component_array::operator=(const component_array& other)
		{
			if (_allocated)
				free(_allocated);

			_element_size = other._element_size;
			_capacity = other._element_size;
			_assigned_id = other._assigned_id;

			_allocated = malloc(_capacity * _element_size);

			_allocated = malloc(_element_size * _capacity);
			for (size_t i = 0; i < _capacity * _element_size; i++)
			{
				((char*)_allocated)[i] = ((char*)other._allocated)[i];
			}

			return *this;
		}

		component_array::~component_array()
		{
			if (_allocated)
				free(_allocated);
		}

		void component_array::insert(size_t entity_id, void* component) noexcept
		{
			grow(entity_id - 1);
			char* per_byte_pointer = ((char*)_allocated) + entity_id * _element_size;

			// set the first 8 bytes to be the entity id
			*((size_t*)per_byte_pointer) = entity_id;

			for (size_t i = 8; i < _element_size; ++i)
			{
				per_byte_pointer[i] = ((char*)component)[i];
			}
		}

		void component_array::insert_new(size_t entity_id, void* component) noexcept
		{
			grow(entity_id - 1);
			insert(entity_id, component);
			free(component);
		}

		void component_array::remove(size_t entity_id) noexcept
		{
			void* element_pointer = at(entity_id);
			if (!element_pointer)
				return;

			for (size_t i = 0; i < _element_size; ++i)
			{
				((char*)element_pointer)[i] = 0;
			}
		}

		void component_array::grow(size_t num_elements) noexcept
		{
			if (num_elements <= _capacity || num_elements == ECS_INVALID_ENTITY) // don't want to grow the array to 2^64 elements, really don't want that
				return;

			size_t grow_strides = num_elements / ECS_COMPONENT_ARRAY_GROW_STRIDE + 1;
			void* allocated_new = malloc(_element_size * grow_strides);
			_capacity = grow_strides * ECS_COMPONENT_ARRAY_GROW_STRIDE;

			// copy memory over
			for (size_t i = 0; i < _capacity / sizeof(size_t); ++i)
			{
				((size_t*)allocated_new)[i] = ((size_t*)_allocated)[i]; // copy groups of 8 bytes
			}
			for (size_t i = grow_strides * ECS_COMPONENT_ARRAY_GROW_STRIDE; i < _capacity; ++i) // copy leftover bytes
			{
				((char*)allocated_new)[i] = ((char*)_allocated)[i];
			}

			free(_allocated);
			_allocated = allocated_new;
		}

		void* component_array::at(size_t entity_id) noexcept
		{
			if (entity_id > _capacity)
				return nullptr;

			// check that the component is not a 0 value
			for (size_t i = 0; i < _element_size; ++i)
			{
				if (((char*)_allocated)[i] != 0)
				{
					return (void*)(((char*)_allocated) + entity_id * _element_size);
				}
			}

			// component has no data, return 0
			// [this might cause issues if the component's data is supposed to be 0 lol]
			return nullptr;
		}

		void* component_array::operator[] (size_t i) noexcept
		{
			return at(i);
		}
	}
}
