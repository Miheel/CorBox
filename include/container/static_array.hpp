#ifndef STATIC_ARRAY_HPP
#define STATIC_ARRAY_HPP

#include "types.hpp"
#include "iter.hpp"
#include "utility.hpp"
#include "memory.hpp"
#include "assert.hpp"

namespace cor
{

	template <typename T, cor::usize _size>
	class Static_Array
	{
	public:
		Static_Array() = default;

		Static_Array(std::initializer_list<T> list)
		{
			assert(list.size() == _size);
			cor::mem::memCopy(list.begin(), list.end(), elems);
		}

		// elem access
		T &operator[](cor::usize pos) { return elems[pos]; }
		const T &operator[](cor::usize pos) const { return elems[pos]; }

		T &front() { return elems[0]; }
		const T &front() const { return elems[0]; }

		T &back() { return elems[_size - 1]; }
		const T &back() const { return elems[_size - 1]; }

		T *data() noexcept { return elems; }
		const T *data() const { return elems; }

		// iterator
		T *begin() noexcept { return elems; }
		const T *begin() const noexcept { return elems; }

		T *end() noexcept { return elems + _size; }
		const T *end() const noexcept { return elems + _size; }

		// capacity
		[[nodiscard]] constexpr bool empty() const noexcept { return false; }
		constexpr cor::usize size() const noexcept { return _size; }

		// operations
		void fill(const T &val)
		{
			for (usize i = 0; i < _size; i++)
			{
				elems[i] = val;
			}
		}
		void swap(Static_Array &other)
		{
			cor::swap(this->elems, other.elems);
		}

	private:
		T elems[_size];
	};
}

#endif // !STATIC_ARRAY_HPP
