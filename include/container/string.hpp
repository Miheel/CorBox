#ifndef STRING_HPP
#define STRING_HPP

#include "utility.hpp"
#include "memory.hpp"

namespace cor {

	class String
	{
	public:
		using pointer = char*;
		using const_pointer = const char*;
		using reference = char&;
		using const_reference = const char&;

		//CTOR
		String() = default;
		String(size_t count, char chr);
		String(const_pointer s);
		String(const_pointer s, size_t count);
		String(const String& other);
		String(String&& other) noexcept;
		String(std::initializer_list<char> ilist);

		//ELEM ACCESS
		reference operator[](size_t index);
		const_reference operator[](size_t index) const;
		reference front();
		const_reference front() const;
		reference back();
		const const_reference back() const;
		pointer data() noexcept;
		const_pointer data() const noexcept;

		//ITER

		//CAP
		constexpr size_t size() const noexcept;

		//MODIFIERS
		constexpr void swap(String& other) noexcept;


		~String();

	private:
		mem::Allocator<char> allocator;
		size_t ssize = 0;
		pointer ptr = nullptr;
	};

}

#endif // !STRING_HPP
