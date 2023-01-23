/*
* Name: Mikael leuf
* Course: Datastrukturer och algoritmer, dt046g
* Date: 2021-05-06
* Lab: Grafer 1
*/

#ifndef TYPEDEF_HPP
#define TYPEDEF_HPP

#include <chrono>
#include <vector>
#include <map>
#include <String>
#include <limits>

namespace cor::time {

	typedef std::chrono::nanoseconds nano;
	typedef std::chrono::microseconds micro;
	typedef std::chrono::milliseconds mili;
	typedef std::chrono::seconds sec;
	typedef std::chrono::steady_clock steadyClock_T;
	typedef std::chrono::system_clock systemClock_T;

	template<typename clock_t>
	using timePoint = std::chrono::time_point<clock_t>;

}// !namespace cor::time

#endif // !TYPEDEF_HPP
