#include "math.hpp"

constexpr int cor::sign(long long num)
{
	return num < 0 ? -1 : 1;
}

cor::usize cor::pow(usize base, usize exponent)
{
	usize product = 1;
	if (exponent == 2)
	{
		product = base * base;
	}
	else
	{
		for (usize i = 1; i <= exponent; i++)
		{
			product *= base;
		}
	}
	return product;
}

std::map<cor::usize, cor::usize> cor::primefactorization(usize n)
{
	std::map<usize, usize> pFacMap;
	while (n % 2 == 0)
	{
		if (pFacMap.find(2) != pFacMap.end())
		{
			pFacMap[2]++;
		}
		else
		{
			pFacMap.insert({2, 1});
		}
		n = n / 2;
	}

	for (usize i = 3; i <= sqrt(n); i++)
	{
		while (n % i == 0)
		{
			if (pFacMap.find(i) != pFacMap.end())
			{
				pFacMap[i]++;
			}
			else
			{
				pFacMap.insert({i, 1});
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
			pFacMap.insert({n, 1});
		}
	}
	return pFacMap;
}

cor::usize cor::phiFunk(usize n)
{
	auto factors = primefactorization(n);
	usize product = 1;
	for (auto &e : factors)
	{
		product *= cor::pow(e.first, e.second - 1) * (e.first - 1);
	}

	return product;
}
