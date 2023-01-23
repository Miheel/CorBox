#ifndef TIMER_HPP
#define TIMER_HPP

#include "typedef.hpp"

namespace cor::time {

	template<typename duration_t, class clock_t>
	class Timer {
	private:
		timePoint<clock_t> startPoint;
		timePoint<clock_t> stopPoint;

	public:

		Timer() = default;

		inline void start() {
			startPoint = clock_t::now();
		}

		inline void stop() {
			stopPoint = clock_t::now();
		}

		double elapsedSec() {
			return std::chrono::duration<double>(stopPoint - startPoint).count();
		}
		double elapsedTime() {
			return double(std::chrono::duration_cast<duration_t>(stopPoint - startPoint).count());
		}
	};

}// !namespace cor::time

#endif // !TIMER_HPP
