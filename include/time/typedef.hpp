#ifndef TYPEDEF_HPP
#define TYPEDEF_HPP

#include <chrono>
#include <vector>
#include <map>
#include <String>
#include <limits>

namespace cor::time {

	using femto = std::chrono::duration<int64_t, std::femto>;
	using pico = std::chrono::duration<int64_t, std::pico>;
	typedef std::chrono::nanoseconds nano;
	typedef std::chrono::microseconds micro;
	typedef std::chrono::milliseconds mili;
	typedef std::chrono::seconds sec;

	constexpr long long millisec = 1000000;
	constexpr long long microsec = 100;
	constexpr long long nanosec = 1;

	typedef std::chrono::steady_clock steadyClock_T;
	typedef std::chrono::system_clock systemClock_T;

	template<typename clock_t>
	using timePoint = std::chrono::time_point<clock_t>;

}// !namespace cor::time

#endif // !TYPEDEF_HPP
