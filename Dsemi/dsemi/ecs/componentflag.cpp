#include "dsemi/ecs/componentflag.h"

#define min(x, y) (x < y ? x : y)
#define max(x, y) (x < y ? y : x)

namespace dsemi {
	namespace ecs {
		component_flag::component_flag(size_t num_components)
			: _flags(nullptr), _size(0), _element_bit_size(sizeof(uint64_t) * 8)
		{
			if (num_components == 0)
				return;

			_size = (num_components - 1) / _element_bit_size + 1;

			_flags = new uint64_t[_size];
			// Initialize flags to 0
			for (size_t i = 0; i < _size; ++i)
			{
				_flags[i] = 0u;
			}
		}

		component_flag::component_flag(const component_flag& other)
			: _element_bit_size(sizeof(uint64_t) * 8)
		{
			_size = other._size;
			if (_size == 0) {
				_flags = nullptr;
				return;
			}

			_flags = new uint64_t[_size];
			for (size_t i = 0; i < _size; i++)
			{
				_flags[i] = other._flags[i];
			}
		}

		component_flag& component_flag::operator=(const component_flag& other)
		{
			if (_size < other._size)
			{
				delete[] _flags;
				_size = other._size;
				_flags = new uint64_t[other._size];
			}

			for (size_t i = 0; i < _size; ++i)
			{
				_flags[i] = other._flags[i];
			}
			for (size_t i = other._size; i < _size; ++i)
			{
				_flags[i] = 0;
			}

			return *this;
		}

		component_flag::~component_flag()
		{
			delete[] _flags;
		}

		bool component_flag::get(size_t id) const
		{
			return id < (_size* _element_bit_size) ? (_flags[id / _element_bit_size] & (1 << id % _element_bit_size)) : false;
		}

		void component_flag::set(size_t id, bool val)
		{
			if (id >= (_size * _element_bit_size))
				resize(id + 1);

			uint64_t& pByte = _flags[id / _element_bit_size];
			pByte = pByte & ~(1 << id) | ((uint64_t)val << id);
		}

		void component_flag::set_element(size_t index, uint64_t val)
		{
			if (index >= _size)
				resize((index + 1) * _element_bit_size);

			_flags[index] = val;
		}

		void component_flag::resize(size_t num_components)
		{
			size_t _sizeNew = (num_components - 1) / _element_bit_size + 1;
			if (_sizeNew == _size)
				return;

			uint64_t* _flagsNew = new uint64_t[_sizeNew];
			for (size_t i = 0; i < min(_size, _sizeNew); i++)
			{
				_flagsNew[i] = _flags[i];
			}

			for (size_t i = _size; i < _sizeNew; i++)
			{
				_flagsNew[i] = 0u;
			}

			delete[] _flags;
			_flags = _flagsNew;
			_size = _sizeNew;
		}

		bool component_flag::matching_all(const component_flag& filter) const {
			for (size_t i = 0; i < min(_size, filter._size); i++)
			{
				if ((_flags[i] & filter._flags[i]) != filter._flags[i]) // not all required bits found
					return false;
			}
			if (_size < filter._size)
			{
				for (size_t i = _size; i < filter._size; i++)
				{
					if (filter._flags[i] != 0) // required bits outside of range, no match
						return false;
				}
			}
			return true;
		}

		bool component_flag::matching_any(const component_flag& filter) const {
			for (size_t i = 0; i < min(_size, filter._size); i++)
			{
				if ((_flags[i] & filter._flags[i]) != 0) // atleast one bit matched
					return true;
			}
			return false;
		}

		bool component_flag::matching_none(const component_flag& filter) const {
			return !matching_any(filter);
		}

		bool component_flag::operator==(const component_flag& other) const
		{
			for (size_t i = 0; i < min(_size, other._size); i++)
			{
				if ((_flags[i] & ~(other._flags[i])) != 0)
					return false;
			}
			if (_size < other._size)
			{
				for (size_t i = _size; i < other._size; i++)
				{
					if (other._flags[i] != 0)
						return false;
				}
			}
			else
			{
				for (size_t i = other._size; i < _size; i++)
				{
					if (_flags[i] != 0)
						return false;
				}
			}
			return true;
		}

		bool component_flag::operator!=(const component_flag& other) const
		{
			return !(*this == other);
		}
	}
}
