#ifndef MEMORY_HPP
#define MEMORY_HPP

#include "algorithms.hpp"
#include "type_traits.hpp"
#include "types.hpp"
#include "utility.hpp"
#include <limits>
#include <new>

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <exception>
#define DBG_NEW new (_NORMAL_BLOCK, __FILE__, __LINE__)

// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif

constexpr auto DEFAULT_ALIGNMENT = 16ull;

namespace cor::mem
{

	enum class alignVal_T : usize
	{
	};

	template <typename T>
	T *allocateArr(usize size)
	{
		if (size > std::numeric_limits<std::size_t>::max() / sizeof(T))
			throw std::bad_array_new_length();
		return size > 0 ? DBG_NEW T[size]{} : DBG_NEW T[size];
	}

	template <typename T>
	void deallocateArr(T *ptr)
	{
		delete[] ptr;
	}

	template <typename T>
	cor::RemoveExtent_T<T> *allocate_r_extent(usize size)
	{
		typedef cor::RemoveExtent_T<T> Elem;
		Elem *alocatedMem = DBG_NEW Elem[size]();
		return alocatedMem;
	}

	template <typename T>
	T *allocate()
	{
		return DBG_NEW T;
	}

	// template <typename T, typename U>
	// T* allocate(const U& value) {
	//	return new T(value);
	// }

	template <typename T, typename... U>
	T *allocate(U &&...value)
	{
		return DBG_NEW T(cor::forward<U>(value)...);
	}

	template <typename T>
	void deallocate(T *ptr)
	{
		delete ptr;
	}

	template <typename T, typename Place, typename... Args>
	T *constructInPlace(Place *place, Args &&...args)
	{
		return ::new (place) T(cor::forward<Args>(args)...);
	}

	template <typename T, typename Place>
	T *defaultConstructInPlace(Place *place)
	{
		return ::new (place) T();
	}

	template <usize Align, EnableIf_T<(Align > DEFAULT_ALIGNMENT), int> = 0>
	void *allocateRaw(usize count)
	{
		return operator new(count, std::align_val_t{Align});
	}

	template <usize Align, EnableIf_T<(Align <= DEFAULT_ALIGNMENT), int> = 0>
	void *allocateRaw(usize count)
	{
		return operator new(count);
	}

	template <usize Align, EnableIf_T<(Align > DEFAULT_ALIGNMENT), int> = 0>
	void deallocateRaw(void *ptr)
	{
		return operator delete(ptr, std::align_val_t{Align});
	}

	template <usize Align, EnableIf_T<(Align <= DEFAULT_ALIGNMENT), int> = 0>
	void deallocateRaw(void *ptr)
	{
		return operator delete(ptr);
	}

	template <class T>
	inline constexpr usize align_of = max_of(alignof(T), static_cast<usize>(DEFAULT_ALIGNMENT));

	template <typename InputIt, typename OutputIt>
	void memCopy(InputIt first, InputIt last, OutputIt d_first)
	{
		for (; first != last; first++, d_first++)
		{
			*d_first = *first;
		}
	}

	template <typename InContainer, typename OutContainer>
	void memCopy(InContainer &source, OutContainer &dest)
	{
		memCopy(source.begin(), source.end(), dest.begin());
	}

	template <typename InputIt, typename OutputIt>
	void memCopyBack(InputIt first, InputIt last, OutputIt d_last)
	{
		while (first != last)
		{
			*(--d_last) = *(--last);
		}
	}

	template <typename InContainer, typename OutContainer>
	void memCopyBack(InContainer &source, OutContainer &dest)
	{
		memCopyBack(source.begin(), source.end(), dest.begin());
	}

	template <typename InputIt, typename OutputIt>
	void memMove(InputIt first, InputIt last, OutputIt d_first)
	{
		if (d_first < first)
		{
			memCopy(first, last, d_first);
		}
		else if (d_first > first)
		{
			auto d_last = d_first + (last - first);
			memCopyBack(first, last, d_last);
		}
	}

	template <typename InContainer, typename OutContainer>
	void memMove(InContainer &source, OutContainer &dest)
	{
		memMove(source.begin(), source.end(), dest.begin());
	}

} // !namespace mem

#endif // !MEMORY_HPP
