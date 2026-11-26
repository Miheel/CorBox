#ifndef MATH_HPP
#define MATH_HPP

#include "array.hpp"
#include "string.hpp"
#include "types.hpp"
#include "utility.hpp"
#include <map>
#include <tuple>

namespace cor
{

	constexpr double PI = 3.14159265358979323846;

	template <typename T>
	constexpr T abs(T value)
	{
		return (value < 0) ? -value : value;
	}

	template <typename T>
	constexpr int sign(T num)
	{
		return num < 0 ? -1 : 1;
	}

	template <typename T>
	T pow(T base, usize exponent)
	{
		T product = 1;
		if (exponent == 2)
		{
			product = base * base;
		}
		else
		{
			for (cor::usize i = 1; i <= exponent; i++)
			{
				product *= base;
			}
		}
		return product;
	}


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
		cor::Array<std::map<cor::usize, cor::usize>> primeFactorVec = { primefactorization(num2), primefactorization(nums)... };
		auto newFactors = primefactorization(num1);
		cor::usize product = 1;
		for (auto& map : primeFactorVec)
		{

			for (auto& e : map)
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
					newFactors.insert({ e.first, e.second });
				}
			}
		}
		for (auto& e : newFactors)
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

	int toDeci(cor::String nr, cor::usize from_b);

	cor::String fromDeci(int nr, cor::usize to_b);

	cor::String baseconvert(cor::String nr, cor::usize from_b, cor::usize to_b);

	double degToRad(double degrees);

	double radToDeg(double radians);
} // !cor::

#endif // !MATH_HPP
