#include "assert.hpp"

void assertCall(const char * file, unsigned int line, const char * func, const char * expr)
{
	printf("%s: %s:%u: Assert Failed on %s", file, func, line, expr);
	exit(EXIT_FAILURE);
}
