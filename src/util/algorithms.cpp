#include "algorithms.hpp"
#include "assert.h"

cor::usize cor::strlen(const char *str)
{
	auto end = str;
	while (*end != '\0')
	{
		end++;
	}
	return static_cast<usize>(end - str);
}

int cor::strcmp(const char *lhs, const char *rhs)
{
	if (lhs && rhs)
	{
		auto left = lhs, right = rhs;
		while (*left != '\0' && *right != '\0')
		{
			if (*left < *right)
			{
				return *left - *right;
			}
			else if (*left > *right)
			{
				return *left - *right;
			}
			left++;
			right++;
		}
		// equal 0
		if (*left == '\0' && *right == '\0')
		{
			return 0;
		}
		// right bigger -1
		else if (*left == '\0' && *right != '\0')
		{
			return -1;
		}
		// left bigger 1
		else if (*left != '\0' && *right == '\0')
		{
			return 1;
		}
	}
	return 1;
}

int cor::ctoi(char chr)
{
	assert((chr >= '0' && chr <= '9') && "chr must be a char between 0 and 9");
	if (chr >= '0' && chr <= '9')
	{
		return chr - '0';
	}
	return -1;
}

char cor::itoc(int i)
{
	if (i >= 0 && i <= 9)
	{
		return i + '0';
	}
	return -1;
}

int cor::alphatoint(char chr)
{
	assert(((chr >= '0' && chr <= '9') || (chr >= 'A' && chr <= 'Z') || (chr >= 'a' && chr <= 'z')) && "char must be 0-9, A-Z or a-z");

	if (chr >= '0' && chr <= '9')
	{
		return ctoi(chr);
	}
	if (chr >= 'A' && chr <= 'Z')
	{
		//'F' - 'A' + 10 = 15
		// 70  - 65  + 10 = 15
		return chr - 'A' + 10;
	}
	if (chr >= 'a' && chr <= 'b')
	{
		return chr - 'a' + 36;
	}
	return -1;
}

char cor::inttoalpha(int i)
{
	if (i >= 0 && i <= 9)
	{
		return itoc(i);
	}

	if (i >= 10 && i <= 35)
	{
		// 15 + 'A' - 10 = 'F'
		// 15 + 65 - 10 = 70
		return i + 'A' - 10;
	}
	if (i >= 36 && i <= 61)
	{
		return i + 'a' - 36;
	}
	return -1;
}
