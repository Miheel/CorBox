#include "string.hpp"

cor::String::String(size_t count, char chr) :ssize(count)
{
	this->ptr = allocator.allocate(count);
	size_t i = 0;
	for (; i < count; i++)
	{
		allocator.construct(this->ptr + i, chr);
	}
	allocator.construct(this->ptr + i, '\0');
}

cor::String::String(const_pointer s)
{
	this->ssize = cor::strlen(s);

	this->ptr = allocator.allocate(this->ssize);
	allocator.constructN(this->ptr, this->ssize + 1);

	mem::memCopy(s, s + this->ssize, this->ptr);
}

cor::String::String(const_pointer s, size_t count)
{
	this->ssize = count;

	this->ptr = allocator.allocate(this->ssize);
	allocator.constructN(this->ptr, this->ssize + 1);

	mem::memCopy(s, s + this->ssize, this->ptr);
}

cor::String::String(const String & other)
	:String(other.data(), other.size())
{}

cor::String::String(String && other) noexcept
{
	this->swap(other);
}

cor::String::String(std::initializer_list<char> ilist)
{
}

cor::String::reference cor::String::operator[](size_t index)
{
	return this->ptr[index];
}
cor::String::const_reference cor::String::operator[](size_t index) const
{
	return this->ptr[index];
}

cor::String::reference cor::String::front()
{
	return this->ptr[0];
}

cor::String::const_reference cor::String::front() const
{
	return this->ptr[0];
}

cor::String::reference cor::String::back()
{
	return this->ptr[this->size()];
}

const cor::String::const_reference cor::String::back() const
{
	return this->ptr[this->size()];
}

constexpr size_t cor::String::size() const noexcept
{
	return this->ssize;
}

cor::String::const_pointer cor::String::data() const noexcept
{
	return this->ptr;
}

cor::String::pointer cor::String::data() noexcept
{
	return this->ptr;
}

constexpr void cor::String::swap(String & other) noexcept
{
	cor::swap(this->ssize, other.ssize);
	cor::swap(this->ptr, other.ptr);
}

cor::String::~String()
{
}
