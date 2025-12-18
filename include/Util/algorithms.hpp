#ifndef ALGORITHMS_HPP
#define ALGORITHMS_HPP

#include "types.hpp"
#include <algorithm>
#include <initializer_list>
#include <range.hpp>

namespace cor
{

	template <class T>
	constexpr const T &max_of(const T &a, const T &b)
	{
		return a < b ? b : a;
	}

	template <class T>
	constexpr const T &max_of(std::initializer_list<T> ilist)
	{
		// fix max element
		return *std::max_element(ilist.begin(), ilist.end());
	}

	template <typename InIt, typename OutIt, typename Func>
	inline OutIt transform(const InIt first, const InIt last, OutIt dest, Func func)
	{
		for (auto tempFirst = first; tempFirst != last; tempFirst++, dest++)
		{
			*dest = func(*tempFirst);
		}
		return dest;
	}

	template <typename T>
	bool cycleDetect(const T &list)
	{

		if (list.begin() == list.end())
		{
			return false;
		}

		auto tortoise = list.begin();
		auto hare = list.begin()->next;

		while (hare != tortoise && hare != nullptr)
		{
			tortoise = tortoise->next;
			hare = hare->next->next;
		}
		if (hare == tortoise)
		{
			return true;
		}
		return false;
	}

	template <class T>
	T &search(cor::Slice<T> slice, const T &value)
	{
		for (auto It = slice.begin(); It != slice.end(); It++)
		{
			if (*It == value)
			{
				return *It;
			}
		}
		return *slice.end();
	}

	cor::usize strlen(const char *str);

	int strcmp(const char *lhs, const char *rhs);

	int ctoi(char chr);
	char itoc(int i);
	int alphatoint(char chr);
	char inttoalpha(int i);

} // !namespace cor

#endif // !ALGORITHMS_HPP