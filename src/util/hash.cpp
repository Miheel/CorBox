#include "hash.hpp"

constexpr cor::usize FNV_OFFSET_BASIS = 14695981039346656037ull;
constexpr cor::usize FNV_prime = 1099511628211ull;

cor::usize cor::hash_FNV_1a(const unsigned char* first, cor::usize count)
{
	cor::usize val = FNV_OFFSET_BASIS;
	for (cor::usize i = 0; i < count; i++)
	{
		val *= FNV_prime;
		val ^= first[i];
	}
	return val;
}
