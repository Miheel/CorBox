#ifndef MEMORY_HPP
#define MEMORY_HPP

#include "utility.hpp"

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

#undef new
#include "MemoryLeakChecker.hpp"


namespace cor::mem {

	template <typename T>
	T* allocateArr(size_t size) {
		return new T[size];
	}

	template <typename T>
	void deallocateArr(T* ptr) {
		delete[] ptr;
	}

	template <typename T>
	T* allocate() {
		return new T;
	}

	//template <typename T, typename U>
	//T* allocate(const U& value) {
	//	return new T(value);
	//}

	template <typename T, typename... U>
	T* allocate(U&&... value) {
		return new T(cor::forward<U>(value)...);
	}


	template <typename T>
	void deallocate(T* ptr) {
		delete ptr;
	}

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
