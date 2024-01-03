#ifndef TIMER_HPP
#define TIMER_HPP

#include <numeric>
#include <ctime>
#include "typedef.hpp"
#ifdef _WIN32
#include <Windows.h>
#else

#endif // _WIN32



namespace cor::time {

	template<typename duration_t, class clock_t = steadyClock_T>
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

#ifdef _WIN32
	//windows spesific function 
	inline long long counter()
	{
		LARGE_INTEGER li;
		QueryPerformanceCounter(&li);
		return li.QuadPart;
	}

	inline long long frequency()
	{
		LARGE_INTEGER li;
		QueryPerformanceFrequency(&li);
		return li.QuadPart;
	}
#endif // _WIN32#""



	class clock {
	public:
		static long long now() {
			//get time in nanosec
#ifdef _WIN32
			auto count = counter();
			auto freq = frequency();
			auto whole_cycles = (count / freq) * 1000000000;
			auto fraction_cycles = (count% freq) * 1000000000 / freq;



#else
			//linux 

#endif
			return whole_cycles + fraction_cycles;

		}
	};

	template<class rep, rep duration>
	class OwnTimer {
		rep startPoint;
		rep stopPoint;
	public:
		OwnTimer() = default;

		inline void start() {
			startPoint = clock::now();
		}
		inline void stop() {
			stopPoint = clock::now();
		}


		double elapsedSec() {
			return static_cast<double>(stopPoint - startPoint) / 1000000000.0;
		}
		double elapsedTime() {
			return double(static_cast<rep>(stopPoint - startPoint) / duration);
		}
	};

}// !namespace cor::time

#endif // !TIMER_HPP
