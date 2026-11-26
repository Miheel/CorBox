#include "assert.hpp"

void assertCall(const char *file, unsigned int line, const char *func, const char *expr)
{
	printf("%s: %s:%u: Assert Failed on %s\n", file, func, line, expr);
	exit(EXIT_FAILURE);
}

void where(const char *file, unsigned int line, const char *func)
{
	printf("where: %s: %s:%u:\n", file, func, line);
}