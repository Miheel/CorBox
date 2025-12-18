#ifndef RANGE_HPP
#define RANGE_HPP

#include "types.hpp"

namespace cor
{

	template <class T>
	class Slice
	{
	public:
		constexpr Slice(const T *s, cor::usize count)
			: first(s), ssize(count)
		{
		}

		Slice(T *first = nullptr, T *last = nullptr)
			: first(first), ssize((last - first))
		{
		}

		cor::usize size() const { return ssize; }
		bool empty() const { return first == nullptr; }

		T &operator[](cor::usize index) const { return first[index]; }
		T &at(cor::usize index) const { return first[index]; }

		const T &front() const { return *first; }
		const T &back() const { return *(first + ssize - 1); }

		T &front() { return *first; }
		T &back() { return *(first + ssize - 1); }

		T *data() { return first; }

		const T *begin() const { return first; }
		const T *end() const { return first + ssize; }

		T *begin() { return first; }
		T *end() { return first + ssize; }

	private:
		T *first = nullptr;
		cor::usize ssize;
	};

	template <class T>
	class View
	{
	public:
		constexpr View(const T *s, cor::usize count)
			: first(s), last(s + count)
		{
		}

		View(T *first = nullptr, T *last = nullptr)
			: first(first), last(last)
		{
		}

		cor::usize size() const { return last - first; }
		bool empty() const { return first == nullptr; }

		const T &operator[](cor::usize index) const { return first[index]; }
		const T &at(cor::usize index) const { return first[index]; }

		const T &front() const { return *first; }
		const T &back() const { return *(last - 1); }

		const T *data() const { return first; }

		const T *begin() const { return first; }
		const T *end() const { return last; }

	private:
		const T *first = nullptr;
		const T *last = nullptr;
	};

}
#endif // !SLICE_HPP