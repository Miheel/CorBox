#ifndef ARRAY_HPP
#define ARRAY_HPP

#include "buffer.hpp"
#include "slice.hpp"
namespace cor {

	template<typename T>
	class Array
	{
	public:
		using reference = T & ;
		using const_reference = const T &;
		using iterator = T * ;
		using const_iterator = const T*;

		//constructors
		Array() : buffer(0) {}
		explicit Array(size_ty size)
			:buffer(size)
		{}
		Array(const T* source, size_ty size)
			:buffer(source, size), currentSize(size)
		{}
		Array(const Array& rhs) //copy constructor
			:Array(rhs, rhs.size())
		{}
		Array(Array&& rhs) { //move constructor
			this->swap(rhs);
		}

		constexpr Array(std::initializer_list<T> init) 
			:buffer(init), currentSize(init.size())
		{}

		//Assign
		Array& operator= (const Array& rhs) { //copy assign
			auto temp(rhs);
			this->swap(temp);
			return *this;
		}
		Array& operator= (Array&& rhs) { //move assign
			this->swap(rhs);
			return *this;
		}

		//Access
		constexpr reference operator[] (size_ty index) { return buffer[index]; }
		constexpr const_reference operator[] (size_ty index) const { return buffer[index]; }

		constexpr reference at(size_ty index) { return buffer[index]; }
		constexpr const_reference at(size_ty index) const { return buffer[index]; }


		iterator data() { return buffer.data(); }
		const_iterator data() const { return buffer.data(); }

		//Capacity
		size_ty size() { return currentSize; }
		bool empty() { return currentSize == 0 ? true : false; }
		size_ty capacity() { return buffer.size(); }
		void reserv(size_ty newSize) {
			if (newSize > this->capacity())
			{
				realoc(newSize);
			}
		}
		void shrinkToFit() {
			realoc(currentSize);
		}

		//Modifiers
		void clear() {
			Array temp(buffer.size());
			this->swap(temp);
		}
		void pushBack(const T& value) {
			if (this->size() == this->capacity())
			{
				expand();
			}
			buffer[currentSize++] = value;
		}
		void pushBack(T&& value) {
			if (this->size() == this->capacity())
			{
				expand();
			}
			buffer[currentSize++] = cor::isMovable(value);
		}
		void popBack() { --currentSize; }
		void resize(size_ty newSize) {
		}

		//Iterators
		const_iterator begin() const { return buffer.begin(); }
		const_iterator end() const { return buffer.begin() + currentSize; }

		iterator begin() { return buffer.begin(); }
		iterator end() { return buffer.begin() + currentSize; }

		Slice<T> slice() {
			return Slice<T>(this->begin(), this->end());
		}
		Slice<T> slice(size_t first, size_t last) {
			return Slice<T>(this->begin() + first, this->begin() + last);
		}

		~Array() = default;

		void swap(Array& other) {
			this->buffer.swap(other.buffer);
			cor::swap(this->currentSize, other.currentSize);
		}


	private:
		void expand() {
			size_ty newSize = static_cast<size_ty>(this->capacity() * 1.5);
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
		void realoc(size_ty newSize) {
			Buffer<T> newBuffer(newSize);
			mem::memCopy(buffer.begin(), buffer.end(), newBuffer.begin());
			buffer.swap(newBuffer);
		}

		Buffer<T> buffer;
		size_ty currentSize = 0;
	};

} // !namespace cor

#endif // !ARRAY_HPP
