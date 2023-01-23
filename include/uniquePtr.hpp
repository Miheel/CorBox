#ifndef UNIQUEPTR_HPP
#define UNIQUEPTR_HPP

namespace cor {

	template<typename T>
	class UniquePtr
	{
	private:
		using Pointer = T * ;
		using Elem_t = T;

	public:
		UniquePtr() :pointer(nullptr) {}

		UniquePtr(Pointer p) :pointer(p) {}

		UniquePtr(const T& other) = delete;

		UniquePtr& operator =(const T& other) = delete;


		Pointer get() const noexcept {
			return pointer;
		}

		Pointer operator->() const noexcept {
			return pointer;
		}

		Elem_t operator*() const noexcept {
			return *get();
		}

		~UniquePtr() = default;


	private:
		Pointer pointer;
	};

} // !namespace cor

#endif // !UNIQUEPTR_HPP
