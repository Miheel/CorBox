#ifndef ARRAY_HPP
#define ARRAY_HPP

#include "buffer.hpp"
#include "range.hpp"
#include "utility.hpp"
#include <initializer_list>
#include <iostream>
#include <types.hpp>

namespace cor
{

	template <typename T>
	class Array
	{
	public:
		// constructors
		constexpr Array() : buffer(0) {}
		constexpr Array(cor::usize size)
			: buffer(size), currentSize(size)
		{
		}
		constexpr Array(const T *source, cor::usize size)
			: buffer(source, size), currentSize(size)
		{
		}
		constexpr Array(usize size, const T &val)
			: Array(size)
		{
			for (usize i = 0; i < size; i++)
			{
				this->buffer[i] = val;
			}
		}
		constexpr Array(const Array &rhs) // copy constructor
			: Array(rhs.data(), rhs.size())
		{
		}
		constexpr Array(Array &&rhs) noexcept
		{ // move constructor
			this->swap(rhs);
		}
		constexpr Array(std::initializer_list<T> init)
			: buffer(init), currentSize(init.size())
		{
		}

		// Assign
		constexpr Array &operator=(const Array &rhs)
		{ // copy assign
			auto temp(rhs);
			this->swap(temp);
			return *this;
		}
		constexpr Array &operator=(Array &&rhs) noexcept
		{ // move assign
			this->swap(rhs);
			return *this;
		}

		// Access
		constexpr T &operator[](cor::usize index) { return buffer[index]; }
		constexpr const T &operator[](cor::usize index) const { return buffer[index]; }

		constexpr T &at(cor::usize index) { return buffer[index]; }
		constexpr const T &at(cor::usize index) const { return buffer[index]; }

		constexpr T *data() { return buffer.data(); }
		constexpr const T *data() const { return buffer.data(); }

		// Capacity
		constexpr cor::usize size() const { return currentSize; }
		constexpr bool empty() { return currentSize == 0 ? true : false; }
		constexpr cor::usize capacity() { return buffer.size(); }
		constexpr void reserve(cor::usize newSize)
		{
			if (newSize > this->capacity())
			{
				realoc(newSize);
			}
		}
		constexpr void shrinkToFit()
		{
			realoc(currentSize);
		}

		// Modifiers
		constexpr void clear()
		{
			for (auto &e : buffer)
			{
				e = T();
			}
			currentSize = 0;
		}
		constexpr void pushBack(const T &value)
		{
			if (this->size() == this->capacity())
			{
				expand();
			}
			buffer[currentSize++] = value;
		}
		constexpr void pushBack(T &&value)
		{
			if (this->size() == this->capacity())
			{
				expand();
			}
			buffer[currentSize++] = cor::isMovable(value);
		}
		constexpr void popBack() { --currentSize; }
		constexpr void resize(cor::usize newSize)
		{
			Array newArr(newSize);

			auto count = newSize < this->size() ? newSize : this->size();

			for (usize i = 0; i < count; i++)
			{
				newArr[i] = cor::isMovable(buffer[i]);
			}

			this->swap(newArr);
		}

		// Iterators
		constexpr const T *begin() const { return buffer.begin(); }
		constexpr const T *end() const { return buffer.begin() + currentSize; }

		constexpr T *begin() { return buffer.begin(); }
		constexpr T *end() { return buffer.begin() + currentSize; }

		constexpr Slice<T> slice()
		{
			return Slice<T>(this->begin(), this->end());
		}
		constexpr Slice<T> slice(usize first, usize last)
		{
			return Slice<T>(this->begin() + first, this->begin() + last);
		}

		~Array()
		{
			currentSize = 0;
		}

		constexpr void swap(Array &other)
		{
			this->buffer.swap(other.buffer);
			cor::swap(this->currentSize, other.currentSize);
		}

	private:
		constexpr void expand()
		{
			cor::usize newSize = static_cast<cor::usize>(this->capacity() * 1.5);
			if (this->capacity() == 0)
			{
				newSize = 1;
			}
			else if (this->capacity() == 1)
			{
				newSize++;
			}

			realoc(newSize);
		}
		constexpr void realoc(cor::usize newSize)
		{
			Buffer<T> newBuffer(newSize);
			for (usize i = 0; i < currentSize; i++)
			{
				newBuffer[i] = buffer[i];
			}
			buffer.swap(newBuffer);
		}

		Buffer<T> buffer;
		cor::usize currentSize = 0;
	};

	template <class T>
	void PrintArr(const Array<T> &arr)
	{
		for (auto &e : arr)
		{
			std::cout << e << ", ";
		}
		std::cout << "\n";
	}

	template <class T>
	void PrintArr(const Array<Array<T>> &arr)
	{

		for (usize row = 0; row < arr.size(); row++)
		{
			for (usize col = 0; col < arr[row].size(); col++)
			{
				std::cout << arr[row][col] << ", ";
			}
			std::cout << "\n";
		}
	}

} // !namespace cor

#endif // !ARRAY_HPP
