#ifndef STRING_HPP
#define STRING_HPP

#include <initializer_list>
#include "allocator.hpp"
#include "range.hpp"
#include "types.hpp"

namespace cor
{
	class String
	{
	public:
		// CTOR
		String() = default;
		String(cor::usize count, char chr);
		String(const char *s);
		String(const char *s, cor::usize count);
		String(const String &other);
		String(String &&other) noexcept;
		String(std::initializer_list<char> ilist);

		~String();

		// ASSIGN
		String &operator=(const String &str);
		String &operator=(String &&str) noexcept;
		String &operator=(const char *s);
		String &operator=(std::initializer_list<char> ilist);

		// ELEM ACCESS
		char &operator[](cor::usize index);
		const char &operator[](cor::usize index) const;
		char &at(cor::usize index);
		const char &at(cor::usize index) const;
		char &front();
		const char &front() const;
		char &back();
		const char &back() const;

		// ITER
		char *begin() noexcept;
		const char *begin() const noexcept;
		char *end() noexcept;
		const char *end() const noexcept;

		// DATA ACCESS
		constexpr char *data() noexcept { return this->ptr; }
		constexpr const char *data() const noexcept { return this->ptr; }

		// CAPACITY
		constexpr cor::usize size() const noexcept { return this->ssize; }
		constexpr cor::usize capacity() const noexcept { return this->cap; }
		constexpr bool empty() const noexcept { return size() == 0; }
		cor::usize max_size() const noexcept;

		// MODIFIERS
		void pushBack(char chr);
		String &insert(cor::usize index, cor::usize count, char chr);
		void resize(cor::usize newSize);
		void swap(String &other) noexcept;

		// SLICE
		Slice<char> slice(cor::usize first, cor::usize last);
		Slice<char> slice();

	private:
		void aloc(cor::usize count);

		cor::usize ssize = 0;
		cor::usize cap = 0;
		char *ptr = nullptr;
		static inline mem::Allocator<char> allocator{};
	};

	bool operator==(const cor::String &lhs, const cor::String &rhs) noexcept;
	bool operator!=(const cor::String &lhs, const cor::String &rhs) noexcept;
	bool operator==(const cor::String &lhs, const char *rhs) noexcept;
	bool operator!=(const cor::String &lhs, const char *rhs) noexcept;
}

#endif // !STRING_HPP
