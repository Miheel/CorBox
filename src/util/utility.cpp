#include "utility.hpp"

size_t cor::strlen(const char * str)
{
	auto end = str;
	while (*end != '\0')
	{
		end++;
	}
	return static_cast<size_t>(end - str);
}
