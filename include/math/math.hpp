#ifndef MATH_HPP
#define MATH_HPP
#include <map>
#include <vector>

namespace cor {

	size_t pow(size_t base, size_t exponent) {
		size_t product = 1;
		if (exponent == 2)
		{
			product = base * base;
		}
		else
		{
			for (size_t i = 1; i <= exponent; i++)
			{
				product *= base;
			}

		}
		return product;
	}

	std::map<size_t, size_t> primefactorization(size_t n) {
		std::map<size_t, size_t> pFacMap;
		while (n % 2 == 0)
		{
			if (pFacMap.find(2) != pFacMap.end())
			{
				pFacMap[2]++;
			}
			else
			{
				pFacMap.insert({ 2,1 });
			}
			n = n / 2;
		}

		for (size_t i = 3; i <= sqrt(n); i++)
		{
			while (n % i == 0)
			{
				if (pFacMap.find(i) != pFacMap.end())
				{
					pFacMap[i]++;
				}
				else
				{
					pFacMap.insert({ i,1 });
				}
				n = n / i;
			}
		}

		if (n > 2)
		{
			if (pFacMap.find(n) != pFacMap.end())
			{
				pFacMap[n]++;
			}
			else
			{
				pFacMap.insert({ n,1 });
			}
		}
		return pFacMap;
	}

	size_t phiFunk(size_t n) {
		auto factors = primefactorization(n);
		size_t product = 1;
		for (auto &e : factors)
		{
			product *= cor::pow(e.first, e.second - 1) * (e.first - 1);
		}

		return product;
	}

	template<class T, class ...Args>
	size_t lcm(const T num1, const T num2, Args... nums) {
		std::vector<std::map<size_t, size_t>>primeFactorVec = { primefactorization(num2), primefactorization(nums)... };
		auto newFactors = primefactorization(num1);
		size_t product = 1;
		for (auto &map : primeFactorVec) {

			for (auto &e : map) {
				if (newFactors.find(e.first) != newFactors.end())
				{
					if (e.second > newFactors.find(e.first)->second)
					{
						auto max = std::max(e.second, newFactors.find(e.first)->second);
						newFactors[e.first] = max;
					}
				}
				else
				{
					newFactors.insert({ e.first,e.second });
				}
			}
		}
		for (auto &e : newFactors)
		{
			product *= cor::pow(e.first, e.second);
		}

		return product;
	}

	//size_t gcd(const int n1, const int n2) {

	//	if (n2 == 0)
	//		return n1;

	//	gcd(n2, n1 % n2);
	//}

	//size_t egcd(const int n1, const int n2) {

	//	if (n2 == 0)
	//		return n1;
	//}

}// !cor::

#endif // !MATH_HPP
