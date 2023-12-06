#ifndef MATH_HPP
#define MATH_HPP

#include <map>
#include <vector>

namespace cor {

	template<class T>
	constexpr const T& max_of(const T& a, const T& b) {
		return a < b ? b : a;
	}

	size_t pow(size_t base, size_t exponent);

	std::map<size_t, size_t> primefactorization(size_t n);

	size_t phiFunk(size_t n);

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
