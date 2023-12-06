#ifndef MEMORY_HPP
#define MEMORY_HPP

#include "Util/utility.hpp"
#include "type_traits.hpp"
#include "math.hpp"

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <iostream>
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )


// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif

//#undef new
//#include "MemoryLeakChecker.hpp"

#define DEFAULT_ALIGNMENT 16ull

namespace cor::mem {
	
	enum class alignVal_T : std::size_t {};

	template <typename T>
	T* allocateArr(size_t size) {
		return DBG_NEW T[size];
	}

	template <typename T>
	void deallocateArr(T* ptr) {
		delete[] ptr;
	}

	template <typename T>
	std::remove_extent_t<T>* allocate_r_extent(size_t size) {
		typedef std::remove_extent_t<T> Elem;
		Elem* alocatedMem = DBG_NEW Elem[size]();
		return alocatedMem;
	}
	
	template <typename T>
	T* allocate() {
		return DBG_NEW T;
	}

	//template <typename T, typename U>
	//T* allocate(const U& value) {
	//	return new T(value);
	//}

	template <typename T, typename... U>
	T* allocate(U&&... value) {
		return DBG_NEW T(cor::forward<U>(value)...);
	}

	template <typename T>
	void deallocate(T* ptr) {
		delete ptr;
	}

	template <typename T, typename Place, typename... Args>
	T* allocatePlace(Place* place, Args&&... args) {
		return ::new (place) T(std::forward<Args>(args)...);
	}

	template<size_t Align, EnableIf_T<(Align > DEFAULT_ALIGNMENT), int> = 0>
	inline void* allocateRaw(size_t count) {
		return operator new(count, alignVal_T{ Align });
	}
	template<size_t Align, EnableIf_T<(Align <= DEFAULT_ALIGNMENT), int> = 0>
	inline void* allocateRaw(size_t count) {
		return operator new(count);
	}

	template<size_t Align, EnableIf_T<(Align > DEFAULT_ALIGNMENT), int> = 0>
	inline void deallocateRaw(void* ptr, size_t n) {
		return operator delete(ptr, n, alignVal_T{ Align });
	}
	template<size_t Align, EnableIf_T<(Align <= DEFAULT_ALIGNMENT), int> = 0>
	inline void deallocateRaw(void* ptr, size_t n) {
		return operator delete(ptr, n);
	}
	
	template<class T>
	inline constexpr size_t align_of = max_of(alignof(T), static_cast<size_t>(DEFAULT_ALIGNMENT));

	template<class T>
	struct Allocator
	{
		using value_type = T;
		using pointer = T * ;
		using const_pointer = const T *;
		using reference = T & ;
		using const_reference = const T & ;
		using size_type = size_t;
		using difference_type = std::ptrdiff_t;

		Allocator() = default;
		constexpr Allocator(const Allocator & other) noexcept = default;
		template<class Other>
		constexpr Allocator(const Allocator<Other> & other) noexcept {}

		[[nodiscard]] pointer allocate(size_type n) {
			return static_cast<pointer>(allocateRaw<align_of<pointer>>(n * sizeof(T)));
		}

		void deallocate(pointer p, size_type n) {
			deallocateRaw<align_of<pointer>>(p, n);
		}

		void construct(pointer p, const_reference val) {
			new(static_cast<void*>(p)) T(val);
		}

		void constructN(pointer p, size_type n) {
			for (size_t i = 0; i < n; i++)
			{
				new(static_cast<void*>(p+i)) T();
			}
		}

		void destroy(pointer p) {
			p->~T();
		}

		~Allocator() = default;
	};

	template<class T>
	struct Deleter
	{
		constexpr Deleter() noexcept = default;

		constexpr void operator()(T* ptr) const {
			deallocate(ptr);
		}
	};

	template<class T>
	struct Deleter<T[]>
	{
		constexpr Deleter() noexcept = default;

		template< class U >
		constexpr void operator()(U* ptr) const {
			deallocateArr(ptr);
		}
	};

	template<typename InputIt, typename OutputIt>
	void memCopy(InputIt first, InputIt last, OutputIt d_first) {
		for (; first != last; first++, d_first++)
		{
			*d_first = *first;
		}
	}

	template<typename InContainer, typename OutContainer>
	void memCopy(InContainer& source, OutContainer& dest) {
		memCopy(source.begin(), source.end(), dest.begin());
	}


	template<typename InputIt, typename OutputIt>
	void memMove(InputIt first, InputIt last, OutputIt d_first) {
		for (; first != last; first++, d_first++)
		{
			*d_first = *first;
		}
	}

	template<typename InContainer, typename OutContainer>
	void memMove(InContainer& source, OutContainer& dest) {
		memMove(source.begin(), source.end(), dest.begin());
	}
} // !namespace mem

#endif // !MEMORY_HPP
