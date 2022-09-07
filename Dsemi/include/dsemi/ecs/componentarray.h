#ifndef HEADER_DSEMI_ECS_COMPONENTARRAYS
#define HEADER_DSEMI_ECS_COMPONENTARRAYS

#include "dsemi/ecs/component.h"

#ifndef ECS_COMPONENT_ARRAY_GROW_STRIDE
// The number of entites to reserve memory for when the array needs to grow
#define ECS_COMPONENT_ARRAY_GROW_STRIDE 4096
#endif

namespace dsemi {
	namespace ecs {
		// i say no to templates
		class component_array {
		public:
			component_array();
			component_array(size_t component_size, size_t component_id);
			component_array(const component_array& other);
			component_array& operator=(const component_array& other);

			~component_array();

			inline bool is_valid() const noexcept {
				return _assigned_id != ECS_INVALID_COMPONENT;
			}

			void insert(size_t entity_id, void* component) noexcept;
			void insert_new(size_t entity_id, void* component) noexcept;
			void remove(size_t entity_id) noexcept;

			void grow(size_t num_elements) noexcept;

			void* at(size_t entity_id) noexcept;
			void* operator[] (size_t i) noexcept;

		private:
			void* _allocated;

			size_t _capacity;     // max entity_id that can fit inside this array
			size_t _element_size; // size of each element
			size_t _assigned_id;
		};
	}
}

#endif