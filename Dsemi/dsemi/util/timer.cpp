#include "dsemi/util/timer.h"

namespace dsemi {
	Timer::Timer()
	{
		last = std::chrono::steady_clock::now();
	}

	float Timer::Peek() const noexcept
	{
		return std::chrono::duration<float>(std::chrono::steady_clock::now() - last).count();
	}

	float Timer::Mark() noexcept
	{
		const auto old = last;
		last = std::chrono::steady_clock::now();
		const std::chrono::duration<float> delta = last - old;
		return delta.count();
	}
}
