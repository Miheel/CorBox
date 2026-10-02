#ifndef MATH_HPP
#define MATH_HPP

#include <map>
#include <string>
#include "array.hpp"
#include "types.hpp"
#include "utility.hpp"
#include "algorithms.hpp"

namespace cor
{

	constexpr double PI = 3.14159265358979323846;

	template <typename T>
	constexpr T abs(T value)
	{
		return (value < 0) ? -value : value;
	}

	constexpr int sign(long long num)
	{
		return num < 0 ? -1 : 1;
	}

	float pow(float base, usize exponent);

	double sqrt(double num);

	template <typename... Args>
	double sum(const Args... nums)
	{
		return static_cast<double>((nums + ...));
	}

	template <typename... Args>
	double product(const Args... nums)
	{
		return static_cast<double>((nums * ...));
	}

	template <typename InputIt, typename OP = cor::Plus<double>>
	auto acumulate(InputIt first, InputIt last, OP const op = OP())
	{
		auto acc = *first;
		first++;
		for (; first != last; first++)
		{
			acc = op(acc, *first);
		}
		return acc;
	}

	cor::Array<cor::usize> factorization(usize n);

	std::map<cor::usize, cor::usize> primefactorization(usize n);

	cor::usize phiFunk(usize n);

	cor::usize phi(usize n);

	template <class T, class... Args>
	cor::usize lcm_prime(const T num1, const T num2, Args... nums)
	{
		cor::Array<std::map<cor::usize, cor::usize>> primeFactorVec = {primefactorization(num2), primefactorization(nums)...};
		auto newFactors = primefactorization(num1);
		cor::usize product = 1;
		for (auto &map : primeFactorVec)
		{

			for (auto &e : map)
			{
				if (newFactors.find(e.first) != newFactors.end())
				{
					if (e.second > newFactors.find(e.first)->second)
					{
						auto max = max_of(e.second, newFactors.find(e.first)->second);
						newFactors[e.first] = max;
					}
				}
				else
				{
					newFactors.insert({e.first, e.second});
				}
			}
		}
		for (auto &e : newFactors)
		{
			product *= cor::pow(e.first, e.second);
		}

		return product;
	}

	usize lcm(int n1, int n2);

	usize gcd(int n1, int n2);

	std::tuple<int, int, int> egcd(int n1, int n2);

	usize factorial(usize n);

	usize semifactorial(usize n);

	usize toDeci(std::string nr, int from_b);

	std::string fromDeci(int nr, int to_b);

	std::string baseconvert(std::string nr, int from_b, int to_b);

	double degToRad(double degrees);

	double radToDeg(double radians);
} // !cor::

#endif // !MATH_HPP
