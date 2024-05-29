#ifndef MEMORY_BITSET_HEADER
#define MEMORY_BITSET_HEADER

#include <cassert>
#ifndef NOBITASSERT
#define BITASSERT(x) assert(x);
#else
#define BITASSERT(x)
#endif

namespace dsemi {

	// TODO!!!!!!: bools are treated as a whole byte please rename this to byteset and use chars and fix everything about this mess (as is done in the bitflag class in the ecs module)
	// in short this whole class is horrible
	class Bitset {
	public:
		Bitset(size_t numBits, bool val = false);
		~Bitset();

		size_t Count() const noexcept;
		void Resize(size_t numBits, bool val = false);

		bool Get(size_t n) const noexcept;
		void Set(size_t n, bool val = true);
		void Unset(size_t n);

		bool& operator[](size_t n);

	private:
		bool* pBits;
		size_t bitCount;
	};

}

#endif