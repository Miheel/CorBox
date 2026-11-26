#include "test.hpp"

std::string Test::to_string(const Report& report)
{
	return "Test failed in file: \"" + std::string(report.file) + \
		"\" in func \"" + std::string(report.func) + \
		"\" on line \"" + std::to_string(report.line) + "\"\n" + \
		"Failed on cond: " + std::string(report.expr) + "\n";
}

void Test::check(bool cond, const char* file, unsigned int line, const char* func, const char* expr)
{
	if (!cond)
	{
		reports.pushBack(Report{file, line, func, expr });
	}
}

void Test::failReport()
{
	for (auto& e : reports)
	{
		std::cout << to_string(e);
	}
}
