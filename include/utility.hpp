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

} // !namespace cor

#endif // !UTILITY_HPP
