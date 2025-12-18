#ifndef ASSERT_HPP
#define ASSERT_HPP

#include <cstdlib>
#include <cstdio>

void assertCall(const char *file, unsigned int line, const char *func, const char *expr);

#ifdef NDEBUG
#define assert(condition) ((void)0)
#else
#define assert(condition) (void)((!!(condition)) || (assertCall(__FILE__, __LINE__, __func__, #condition), 0))
#endif

#endif // !ASSERT_HPP
