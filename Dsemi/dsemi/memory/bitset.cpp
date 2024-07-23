#include "Dsemi/Memory/Bitset.h"

namespace dsemi {

	Bitset::Bitset(size_t numBits, bool val)
		: bitCount(numBits)
	{
		BITASSERT(numBits != 0);

		pBits = new bool[numBits];

		// initialize all but last bytes
		for (size_t i = 0; i < bitCount; i++) 
		{
			pBits[i] = val;
		}
	}

	Bitset::~Bitset()
	{
		delete[] pBits;
	}

	size_t Bitset::Count() const 
	{
		return bitCount;
	}

	void Bitset::Resize(size_t numBits, bool val)
	{
		BITASSERT(numBits != 0);
		if (numBits == bitCount)
			return;
		// TODO: Sketch this bitch out so the implementation isn't SHIT

		bool* pNew = new bool[numBits];
		// copy data to new memory location
		for (size_t i = 0; i < (bitCount < numBits ? bitCount : numBits); i++)
		{
			pNew[i] = pBits[i];
		}
		// initialize new bits if growing
		for (size_t i = bitCount; i < (bitCount < numBits ? numBits : 0); i++)
		{
			pNew[i] = val;
		}
		// free old memory
		delete[] pBits;
		// set pointer to new memory
		pBits    = pNew;
		bitCount = numBits;
	}

	bool Bitset::Get(size_t n) const {
		return n < bitCount ? pBits[n] : false;
	}

	void Bitset::Set(size_t n, bool val) {

	}

	void Bitset::Unset(size_t n) {

	}

	bool& Bitset::operator[](size_t n)
	{
		return pBits[n];
	}
}
