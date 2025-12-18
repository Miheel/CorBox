#ifndef ALLOCATOR_HPP
#define ALLOCATOR_HPP

#include "memory.hpp"
#include <types.hpp>

namespace cor::mem {

	template <class T>
	struct Allocator
	{

		Allocator() = default;
		template <class Other>
		constexpr Allocator(const Allocator<Other>& other) noexcept {}

		// Allocate uninitialized storage for n objects of type T.
		[[nodiscard]] T* allocate(usize n)
		{
			return static_cast<T*>(allocateRaw<align_of<T&>>(n * sizeof(T&)));
		}

		// Deallocate previously allocated storage.
		void deallocate(T* p) noexcept
		{
			deallocateRaw<align_of<T&>>(p);
		}

		///
		void construct(T* p, const T& val)
		{
			constructInPlace<T>(p, val);
		}

		// Default-construct n objects at p[0] through p[n-1].
		void constructN(T* p, usize n)
		{
			for (usize i = 0; i < n; i++)
			{
				defaultConstructInPlace<T>(p + i);
			}
		}

		[[nodiscard]] T* create(usize n, const T& val)
		{
			T* tmp_ptr = allocate(n);
			construct(tmp_ptr, val);
			return tmp_ptr;
		}

		[[nodiscard]] T* createN(usize n)
		{
			T* tmp_ptr = allocate(n);
			constructN(tmp_ptr, n);
			return tmp_ptr;
		}

		void destroy(T* p) noexcept
		{
			p->~T();
		}

		~Allocator() = default;
	};

	template <class T>
	struct Deleter
	{
		constexpr Deleter() noexcept = default;

		constexpr void operator()(T* ptr) const
		{
			deallocate(ptr);
		}
	};

	template <class T>
	struct Deleter<T[]>
	{
		constexpr Deleter() noexcept = default;

		template <class U>
		constexpr void operator()(U* ptr) const
		{
			deallocateArr(ptr);
		}
	};
}
#endif // !ALLOCATOR_HPP
