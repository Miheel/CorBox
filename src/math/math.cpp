#include "math.hpp"
#include "assert.hpp"
#include <cmath>

float cor::pow(float base, cor::usize exponent)
{
	float product = 1;
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

double cor::sqrt(double num)
{
	assert(cor::sign(num) == 1);
	double estimate = num / 2, newestimate = 0;
	while (true)
	{
		newestimate = 0.5 * (estimate + (num / estimate));
		if (std::abs(estimate - newestimate) < 1e-9)
		{
			break;
		}
		estimate = newestimate;
	}
	return estimate;
}

std::vector<cor::usize> cor::factorization(usize n)
{
	std::vector<usize> factor_list;
	size_t p = std::sqrt(n);

	for (size_t i = 2; i <= p; i++)
	{
		while (n % i == 0)
		{
			factor_list.push_back(i);
			n = n / i;
		}
	}
	if (factor_list.empty())
	{
		factor_list.push_back(n);
	}
	return factor_list;
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
	cor::usize product = 1;
	for (auto &e : factors)
	{
		product *= cor::pow(e.first, e.second - 1) * (e.first - 1);
	}

	return product;
}

cor::usize cor::phi(usize n)
{
	auto product = n;

	for (size_t i = 2; i * i <= n; i++)
	{
		// for each prime up to sqrt of n
		// if i is a factor of n
		if (n % i == 0)
		{
			// remove all multiples of the same factor
			while (n % i == 0)
			{
				n = n / i;
			}
			// EX Prime factors of n = 30: 2, 3, 5
			// So the formula gives :
			//  𝜙(30) = 30 * (1-1/2) * (1-1/3) * (1-1/5) =
			//  30 * (2/2-1/2) * (3/3-1/3) * (5/5-1/5) =
			//  30 * 1/2 * 2/3 * 4/5 =
			//  30/2 * 2/3 * 4/5 = 15 * 2/3 * 4/5 =
			//  30/3 * 4/5 = 10 * 4/5 =
			//  40 / 5 = 8

			// product * (1 - 1 / p)
			product = product - product / i;
		}
	}
	// if theres anything left in n it must be a prime factor
	// so aply the same logic one more time for that factor
	if (n > 1)
	{
		product = product - product / n;
	}
	return product;
}

cor::usize cor::gcd(int n1, int n2)
{
	if (n1 < n2)
	{
		cor::swap(n1, n2);
	}
	while (n2 > 0)
	{
		auto r = n1 % n2;
		n1 = n2;
		n2 = r;
	}
	return n1;
}

std::tuple<int, int, int> cor::egcd(int n1, int n2)
{
	auto a = n1, b = n2;
	auto s0 = 1, t0 = 0; // Coefficients for a: a = a0 * s0 + b0 * t0
	auto s1 = 0, t1 = 1; // Coefficients for b: b = a0 * s1 + b0 * t1

	while (b > 0)
	{
		int q = a / b, r = a - b * q;
		// Derivation:
		// r = a - b * q
		// r = (a * s0 + b * t0) - q * (a * s1 + b * t1)
		// r = a * s0 + b * t0 - q * a * s1 - q * b * t1
		// r = (s0 - q * s1) * a + (t0 - q * t1) * b

		a = b;
		b = r;
		int s = s0 - q * s1;
		int t = t0 - q * t1;

		/*Coefficients new a*/ s0 = s1, t0 = t1;
		/*Coefficients new b "r" */ s1 = s;
		t1 = t;
	}
	// a is gcd(n1,n2)
	// Bézout coefficients: s0 and t0. so that n1 * s0 + n2 * t0 = a
	return {a, s0, t0};
}

cor::usize cor::toDeci(std::string nr, int from_b)
{
	size_t j = nr.size() - 1, sum_d = 0;
	for (size_t i = 0; i < nr.size(); i++, j--)
	{
		sum_d += cor::alphatoint(nr[i]) * std::pow(from_b, j);
	}
	return sum_d;
}

std::string cor::fromDeci(int nr, int to_b)
{
	std::string newNr = "";
	int a = nr, b = to_b;
	while (a > 0)
	{
		int q = a / b;
		int r = a % b;
		newNr.insert(0, 1, cor::inttoalpha(r));
		a = q;
	}
	return newNr;
}

std::string cor::baseconvert(std::string nr, int from_b, int to_b)
{
	auto deci = toDeci(nr, from_b);
	auto newbase = fromDeci(deci, to_b);
	return newbase;
}
