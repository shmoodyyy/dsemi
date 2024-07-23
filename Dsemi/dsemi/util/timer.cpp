#include "dsemi/util/timer.h"

namespace dsemi {
	Timer::Timer()
	{
		last = std::chrono::steady_clock::now();
	}

	float Timer::Peek() const
	{
		return std::chrono::duration<float>(std::chrono::steady_clock::now() - last).count();
	}

	float Timer::Mark()
	{
		const auto old = last;
		last = std::chrono::steady_clock::now();
		const std::chrono::duration<float> delta = last - old;
		return delta.count();
	}
}
