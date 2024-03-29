#ifndef MATH_HPP
#define MATH_HPP

#include <map>
#include <vector>
#include <algorithm>
#include "types.hpp"

namespace cor
{

	template <typename T>
	constexpr T abs(T value)
	{
		return (value < 0) ? -value : value;
	}

	constexpr int sign(long long num);

	usize pow(usize base, usize exponent);

	std::map<usize, usize> primefactorization(usize n);

	usize phiFunk(usize n);

	template <class T, class... Args>
	usize lcm(const T num1, const T num2, Args... nums)
	{
		std::vector<std::map<usize, usize>> primeFactorVec = {primefactorization(num2), primefactorization(nums)...};
		auto newFactors = primefactorization(num1);
		usize product = 1;
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

	// usize gcd(const int n1, const int n2) {
	//
	//	if (n2 == 0)
	//		return n1;
	//
	//	gcd(n2, n1 % n2);
	// }
	//
	// usize egcd(const int n1, const int n2) {
	//
	//	if (n2 == 0)
	//		return n1;
	// }

} // !cor::

#endif // !MATH_HPP
