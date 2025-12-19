#ifndef STATIC_ARRAY_HPP
#define STATIC_ARRAY_HPP

#include "utility.hpp"
#include "memory.hpp"
#include "types.hpp"
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

		Static_Array(const Static_Array &arr)
		{
			assert(arr.size() == _size);
			cor::mem::memCopy(arr.begin(), arr.end(), elems);
		}

		Static_Array(Static_Array &&arr)
		{
			assert(arr.size() == _size);
			this->swap(arr);
		}

		constexpr Static_Array &operator=(const Static_Array &rhs)
		{ // copy assign
			auto temp(rhs);
			this->swap(temp);
			return *this;
		}
		constexpr Static_Array &operator=(Static_Array &&rhs) noexcept
		{ // move assign
			this->swap(rhs);
			return *this;
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
			for (cor::usize i = 0; i < _size; i++)
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
