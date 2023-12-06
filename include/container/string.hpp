#ifndef STRING_HPP
#define STRING_HPP

#include "utility.hpp"
#include "memory.hpp"
#include <initializer_list>

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

		//ASSIGN
		String& operator=(const String& str);
		String& operator=(String&& str) noexcept;
		String& operator=(const_pointer s);
		String& operator=(char ch);
		String& operator=(std::initializer_list<char> ilist);

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

		//OPERATIONS


		~String();

	private:
		mem::Allocator<char> allocator;
		size_t ssize = 0;
		pointer ptr = nullptr;
	};

	bool operator==(const cor::String& lhs, const cor::String& rhs) noexcept;
	bool operator!=(const cor::String& lhs,const cor::String& rhs) noexcept;
	bool operator==(const cor::String& lhs, const char* rhs) noexcept;
	bool operator!=(const cor::String& lhs, const char* rhs) noexcept;
}

#endif // !STRING_HPP
