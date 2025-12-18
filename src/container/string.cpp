#include "string.hpp"
#include "iter.hpp"
#include "utility.hpp"
#include <algorithms.hpp>
#include <initializer_list>
#include <range.hpp>
#include <types.hpp>

// CTOR
cor::String::String(usize count, char chr)
	: ssize(count)
{
	this->aloc(count);
	usize i = 0;
	for (; i < count; i++)
	{
		this->ptr[i] = chr;
	}
	this->ptr[this->ssize] = '\0';
}

cor::String::String(const char *s)
	: String(s, cor::strlen(s))
{
}

cor::String::String(const char *s, usize count)
	: ssize(count)
{
	this->aloc(count);
	// mem::memCopy(s, s + this->ssize, this->ptr);
	usize i = 0;
	for (; i < count; i++)
	{
		this->ptr[i] = s[i];
	}
	this->ptr[this->ssize] = '\0';
}

cor::String::String(const String &other)
	: String(other.data(), other.size())
{
}

cor::String::String(String &&other) noexcept
{
	this->swap(other);
}

cor::String::String(std::initializer_list<char> ilist)
	: String(ilist.begin(), ilist.size())
{
}

// DESTRUCTOR
cor::String::~String()
{
	allocator.deallocate(this->ptr);
}

// ASSIGN
cor::String &cor::String::operator=(const String &str)
{
	if (*this != str)
	{
		auto temp(str);
		this->swap(temp);
	}
	return *this;
}

cor::String &cor::String::operator=(String &&str) noexcept
{
	if (*this != str)
	{
		this->swap(str);
	}
	return *this;
}

cor::String &cor::String::operator=(const char *s)
{
	if (*this != s)
	{
		cor::String temp(s);
		this->swap(temp);
	}
	return *this;
}

cor::String &cor::String::operator=(std::initializer_list<char> ilist)
{
	cor::String temp(ilist);
	this->swap(temp);
	return *this;
}

// ELEM ACCESS
char &cor::String::operator[](usize index)
{
	return this->data()[index];
}

const char &cor::String::operator[](usize index) const
{
	return this->data()[index];
}

char &cor::String::at(usize index)
{
	return this->data()[index];
}

const char &cor::String::at(usize index) const
{
	return this->data()[index];
}

char &cor::String::front()
{
	return this->data()[0];
}

const char &cor::String::front() const
{
	return this->data()[0];
}

char &cor::String::back()
{
	return this->data()[this->size() - 1];
}

const char &cor::String::back() const
{
	return this->data()[this->size() - 1];
}

// ITER
char *cor::String::begin() noexcept
{
	return this->data();
}

const char *cor::String::begin() const noexcept
{
	return this->data();
}

char *cor::String::end() noexcept
{
	return this->data() + this->size();
}

const char *cor::String::end() const noexcept
{
	return this->data() + this->size();
}

// CAP
cor::usize cor::String::max_size() const noexcept
{
	return cor::distance(this->begin(), this->end());
}

// MODIFIERS
void cor::String::pushBack(char chr)
{
	if (this->size() == this->capacity())
	{
		resize(this->cap + 1);
	}

	this->ptr[this->ssize++] = chr;
	this->ptr[this->ssize] = '\0';
}

cor::String &cor::String::insert(usize index, usize count, char chr)
{
	String temp = "";
	if (index <= this->size())
	{
		// for (size_t i = 0; i < length; i++)
		//{

		//}
	}
	return *this;
}

void cor::String::resize(usize newSize)
{
	usize newCap = newSize <= 15 ? 15 : (((newSize) + 14) / 15) * 15;
	auto temp = allocator.createN(newCap + 1);
	for (size_t i = 0; i < this->ssize; i++)
	{
		temp[i] = this->ptr[i];
	}
	temp[this->ssize] = '\0';
	allocator.deallocate(this->ptr);
	this->ptr = temp;
	this->cap = newCap;
}

void cor::String::swap(String &other) noexcept
{
	cor::swap(this->ssize, other.ssize);
	cor::swap(this->cap, other.cap);
	cor::swap(this->ptr, other.ptr);
}

// SLICE
cor::Slice<char> cor::String::slice(usize first, usize last)
{
	return Slice<char>(this->begin() + first, this->begin() + last);
}

cor::Slice<char> cor::String::slice()
{
	return Slice<char>(this->begin(), this->end());
}

void cor::String::aloc(usize count)
{
	this->cap = count <= 15 ? 15 : ((count + 14) / 15) * 15;
	this->ptr = allocator.createN(this->cap + 1);
	this->ptr[0] = '\0';
}

bool cor::operator==(const cor::String &lhs, const cor::String &rhs) noexcept
{
	return cor::strcmp(lhs.data(), rhs.data()) == 0;
}

bool cor::operator!=(const cor::String &lhs, const cor::String &rhs) noexcept
{
	return cor::strcmp(lhs.data(), rhs.data()) != 0;
}

bool cor::operator==(const cor::String &lhs, const char *rhs) noexcept
{
	return cor::strcmp(lhs.data(), rhs) == 0;
}

bool cor::operator!=(const cor::String &lhs, const char *rhs) noexcept
{
	return cor::strcmp(lhs.data(), rhs) != 0;
}
