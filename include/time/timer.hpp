#ifndef TIMER_HPP
#define TIMER_HPP

#include "typedef.hpp"
#ifdef _WIN32
#include <Windows.h>
#else
#include <time.h>
#endif // _WIN32

namespace cor::time
{

	template <typename duration_t, class clock_t = steadyClock_T>
	class Timer
	{
	private:
		timePoint<clock_t> startPoint;
		timePoint<clock_t> stopPoint;

	public:
		Timer() = default;

		inline void start()
		{
			startPoint = clock_t::now();
		}

		inline void stop()
		{
			stopPoint = clock_t::now();
		}

		double elapsedSec()
		{
			return std::chrono::duration<double>(stopPoint - startPoint).count();
		}
		double elapsedTime()
		{
			return double(std::chrono::duration_cast<duration_t>(stopPoint - startPoint).count());
		}
	};

#ifdef _WIN32
	// windows spesific function
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

	class clock
	{
		static const long long rep = 1000000000;

	public:
		static long long now()
		{
			// get time in nanosec
#ifdef _WIN32
			auto count = counter();
			auto freq = frequency();
			auto whole_cycles = (count / freq) * rep;
			auto fraction_cycles = (count % freq) * rep / freq;
			return whole_cycles + fraction_cycles;
#else
			// linux
			timespec tp;
			clock_gettime(CLOCK_MONOTONIC, &tp);
			return static_cast<long long>(tp.tv_sec) * rep + tp.tv_nsec;
#endif
		}
	};

	template <class rep = long long, rep duration = 1000000>
	class OwnTimer
	{
		rep startPoint = 0;
		rep stopPoint = 0;

	public:
		OwnTimer() = default;

		inline void start()
		{
			startPoint = static_cast<rep>(clock::now());
		}
		inline void stop()
		{
			stopPoint = static_cast<rep>(clock::now());
		}

		double elapsedSec()
		{
			return static_cast<double>(stopPoint - startPoint) / 1000000000.0;
		}
		double elapsedTime()
		{
			return static_cast<double>(stopPoint - startPoint) / static_cast<double>(duration);
		}
	};

	using NanoTimer = OwnTimer<long long, cor::time::nanosec>;
	using MicroTimer = OwnTimer<long long, cor::time::microsec>;
	using MilliTimer = OwnTimer<long long, cor::time::millisec>;

	void sleep_for(long long duration)
	{
		MilliTimer timer;
		timer.start();
		while (timer.elapsedTime() < duration)
		{
			// busy wait
			timer.stop();
		}
	}

} // !namespace cor::time

#endif // !TIMER_HPP
