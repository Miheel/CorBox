#ifndef TIMER_HPP
#define TIMER_HPP

#include <numeric>
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
			auto point = stopPoint - startPoint;
			return std::chrono::duration<double>(point).count();
		}
		double elapsedTime() {
			return double(std::chrono::duration_cast<duration_t>(stopPoint - startPoint).count());
		}
	};

	//constexpr int sign(long long num) {
	//	return num < 0 ? -1 : 1;
	//}

	//template<long long Num, long long Denom = 1>
	//class ratio {
	//public:
	//	static constexpr long long num = sign(Denom)*Num / std::gcd(Num, Denom);

	//	static constexpr long long denom = std::abs(Denom) / std::gcd(Num, Denom);
	//};

	//class clock {

	//public:
	//	using rep = long long;
	//	using scale = ratio<1, 1000000000>;

	//	//using t_point = Time_Point<clock>;
	//	static long long now() {
	//		//get time in nanosec
	//			return (counter()*scale::denom) / frequency();
	//	}

	//private:
	//	static inline long long counter()
	//	{
	//		LARGE_INTEGER li;
	//		QueryPerformanceCounter(&li);
	//		return li.QuadPart;
	//	}

	//	static inline long long frequency()
	//	{
	//		LARGE_INTEGER li;
	//		QueryPerformanceFrequency(&li);
	//		return li.QuadPart;
	//	}
	//};

	////template<class rep>
	//class OwnTimer {
	//	long long startPoint;
	//	long long stopPoint;
	//public:
	//	OwnTimer() = default;

	//	inline void start() {
	//		startPoint = clock::now();
	//	}
	//	inline void stop() {
	//		stopPoint = clock::now();
	//	}


	//	double elapsedSec() {
	//		return static_cast<double>(stopPoint - startPoint);
	//	}
	//	double elapsedTime() {
	//		//return double(std::chrono::duration_cast<duration_t>(stopPoint - startPoint).count());
	//	}

	//private:

	//};

}// !namespace cor::time

#endif // !TIMER_HPP
