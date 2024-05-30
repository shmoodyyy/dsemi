#ifndef HEADER_DSEMI_ECS_COMPONENTFLAG
#define HEADER_DSEMI_ECS_COMPONENTFLAG
#include <cstdint>

namespace dsemi {
	namespace ecs {
		class component_flag {
		public:
			// TODO: replace elementBitSize with local scope values
			component_flag()
				: _flags(nullptr), _size(0), _element_bit_size(sizeof(uint64_t) * 8) {}
			component_flag(size_t num_components);
			component_flag(const component_flag& other);
			component_flag& operator=(const component_flag& other);
			~component_flag();

			void resize(size_t numComponents);

			// Per bit access
			bool get(size_t id) const noexcept;
			void set(size_t id, bool val);
			void set_element(size_t index, uint64_t val);

			// Comparisons relevant to queries
			bool matching_all(const component_flag& other)  const noexcept;
			bool matching_any(const component_flag& other)  const noexcept;
			bool matching_none(const component_flag& other) const noexcept;

			// operator comparison
			bool operator==(const component_flag& other) const noexcept;
			bool operator!=(const component_flag& other) const noexcept;

		private:
			uint64_t* _flags;
			size_t _element_bit_size;
			size_t _size;
		};
	}
}

#endif
