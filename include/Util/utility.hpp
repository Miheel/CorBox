#ifndef UTILITY_HPP
#define UTILITY_HPP

#include "type_traits.hpp"

namespace cor {

	template<typename T>
	constexpr cor::RemoveReference_T<T> && isMovable(T&& t) noexcept {
		return static_cast<cor::RemoveReference_T<T>&&>(t);
	}

	template<typename T>
	constexpr T&& forward(cor::RemoveReference_T<T>&& t) noexcept {
		return static_cast<T&&>(t);
	}

	template<class T>
	constexpr T&& forward(cor::RemoveReference_T<T>& t) noexcept {
		return static_cast<T&&>(t);
	}

	template<typename T>
	constexpr void moveSwap(T& first, T& second) noexcept
	{
		T temp(isMovable(first));
		first = isMovable(second);
		second = isMovable(temp);
	}

	template <typename T>
	constexpr void swap(T &first, T &second) noexcept
    {
        auto temp = first;
        first = second;
        second = temp;
    }

    template <typename Iter>
    constexpr void iterSwap(Iter first, Iter second)
    {
        cor::moveSwap(*first, *second);
    }

	size_t strlen(const char* str);

} // !namespace cor

#endif // !UTILITY_HPP
