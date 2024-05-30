#include "dsemi/math/random.h"
#include <random>

namespace dsemi {

	namespace Random {
		float Float(float min, float max) noexcept
		{
			std::random_device rng;
			std::mt19937 gen(rng());
			std::uniform_real_distribution<> dis(min, max);

			return dis(gen);
		}

		float Float() noexcept
		{
			return Float(0.0f, 1.0f);
		}

		float NormalFloat(float min, float max) noexcept
		{
			std::default_random_engine gen;
			std::normal_distribution<float> dis(min, max);

			return dis(gen);
		}

		float NormalFloat() noexcept
		{
			return NormalFloat(0.0f, 1.0f);
		}
	}
}
