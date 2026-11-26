#ifndef TEST_HPP
#define TEST_HPP

#include "array.hpp"
#include "string.hpp"
#include <string>
#include <iostream>

namespace Test
{
	struct Report
	{
		const char* file;
		unsigned int line;
		const char* func;
		const char* expr;
	};
	std::string to_string(const Report &report);
	static cor::Array<Report> reports;

	void check(bool cold, const char* file, unsigned int line, const char* func, const char* expr);
	void failReport();

}

#define CHECK(cond) Test::check((cond), __FILE__, __LINE__, __func__, #cond)
#endif // !TEST_HPP
