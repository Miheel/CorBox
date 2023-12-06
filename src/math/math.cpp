#include "math.hpp"

size_t cor::pow(size_t base, size_t exponent)
{
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

std::map<size_t, size_t> cor::primefactorization(size_t n)
{
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

size_t cor::phiFunk(size_t n)
{
	auto factors = primefactorization(n);
	size_t product = 1;
	for (auto &e : factors)
	{
		product *= cor::pow(e.first, e.second - 1) * (e.first - 1);
	}

	return product;
}
