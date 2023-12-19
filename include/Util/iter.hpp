#ifndef ITER_HPP
#define ITER_HPP

namespace cor {

	template< class InputIt>
	constexpr void advance(InputIt& it, int n) {
		it += n;
	}

	template< class InputIt >
	constexpr InputIt next(InputIt it, int n = 1) {
		advance(it, n);
		return it;
	}

	template< class InputIt >
	constexpr InputIt prev(InputIt it, int n = 1) {
		advance(it, -n);
		return it;
	}

	template< class InputIt >
	constexpr long long distance(InputIt first, InputIt last) {
		return last - first;
	}
}

#endif // !ITER_HPP