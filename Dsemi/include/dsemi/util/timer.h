#pragma once

#include <chrono>

namespace dsemi {
	// Might add a central TimerManager object to allow for setting up timed events using threads
	class Timer
	{
	public:
		// Per-Tag Timers
		Timer();
		float Peek() const noexcept;
		float Mark() noexcept;

	private:
		std::chrono::steady_clock::time_point last;
	};
}