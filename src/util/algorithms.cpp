#include "algorithms.hpp"

size_t cor::strlen(const char * str)
{
	auto end = str;
	while (*end != '\0')
	{
		end++;
	}
	return static_cast<size_t>(end - str);
}

int cor::strcmp(const char * lhs, const char * rhs)
{
	auto left = lhs, right = rhs;
	while (*left != '\0' && *right != '\0')
	{
		if (*left < *right) {
			return *left - *right;
		}
		else if (*left > *right) {
			return *left - *right;
		}
		left++;
		right++;
	}
	if (*left == '\0' && *right == '\0')
	{
		return 0;
	}
	else if (*left == '\0' && *right != '\0')
	{
		return -1;
	}
	else if (*left != '\0' && *right == '\0')
	{
		return 1;
	}
	return 0;
}
