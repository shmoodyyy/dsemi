#pragma once

namespace dsemi {
	namespace Random {
		float Float(float min, float max) noexcept;
		float Float() noexcept;

		float NormalFloat(float min, float max) noexcept;
		float NormalFloat() noexcept;
	}
}