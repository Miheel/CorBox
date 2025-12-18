#include "types.hpp"
#ifndef ITER_HPP
#define ITER_HPP

namespace cor
{

	template <class InputIt>
	constexpr void advance(InputIt& it, int n)
	{
		it += n;
	}

	template <class InputIt>
	constexpr InputIt next(InputIt it, int n = 1)
	{
		advance(it, n);
		return it;
	}

	template <class InputIt>
	constexpr InputIt prev(InputIt it, int n = 1)
	{
		advance(it, -n);
		return it;
	}

	template <class InputIt>
	constexpr long long distance(InputIt first, InputIt last)
	{
		return last - first;
	}

	template <class T, usize N>
	constexpr T* begin(T(&array)[N]) noexcept
	{
		return (array);
	}

	template <class T, usize N>
	constexpr T* end(T(&array)[N]) noexcept
	{
		return (array + N);
	}

	template <class T, usize N>
	constexpr usize size(T(&array)[N]) noexcept
	{
		return N;
	}
}

#endif // !ITER_HPP
