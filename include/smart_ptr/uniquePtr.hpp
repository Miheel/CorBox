#ifndef UNIQUEPTR_HPP
#define UNIQUEPTR_HPP


#include "utility"
#include "memory"
#include "type_traits"
#include "types.hpp"

namespace cor {

	template<class T, class Deleter = mem::Deleter<T>>
	class UniquePtr
	{	
	public:
		using deleterNoRef = typename cor::RemoveReference_T<Deleter>;
		using Pointer = typename cor::addPointer_T<T, deleterNoRef>;
		using Elem_t = T;

		//Constructors
		UniquePtr() :pointer(nullptr) {}
		UniquePtr(Pointer p) :pointer(p) {}

		//assigns
		UniquePtr& operator =(UniquePtr&& other) noexcept {
			{
				this->reset(other.release());
				deleter = cor::forward<Deleter>(other.getDeleter());
				return *this;
			}
		}

		//deleted functionality
		UniquePtr(const UniquePtr& other) = delete;
		UniquePtr& operator =(const UniquePtr& other) = delete;

		//Modifiers
		Pointer release() noexcept {
			auto oldPtr = this->pointer;
			this->pointer = nullptr;

			return oldPtr;
		}
		void reset(Pointer ptr = Pointer()) noexcept {
			auto oldPtr = this->pointer;
			this->pointer = ptr;

			if (oldPtr)
			{
				this->getDeleter()(oldPtr);
			}
		}
		void swap(UniquePtr& other) noexcept {
			cor::swap(this->pointer, other.pointer);
			cor::swap(this->deleter, other.deleter);
		}

		//Observers
		Pointer get() const noexcept {
			return pointer;
		}
		Deleter& getDeleter() {
			return deleter;
		}
		explicit operator bool() const noexcept {
			return this->get() != nullptr;
		}

		//dereferences pointer to the managed object
		Pointer operator->() const noexcept {
			return pointer;
		}
		Elem_t operator*() const noexcept {
			return *get();
		}

		//Array version, unique_ptr<T[]>
		Elem_t& operator[](cor::size_t i) const {
			return this->get()[i];
		}

		//Destructor
		~UniquePtr() {
			this->getDeleter()(get());
		}


	private:
		Pointer pointer;
		Deleter deleter;
	};

	template<class T, class ...Args>
	constexpr UniquePtr<T> makeUnique(Args && ...args)
	{
		return UniquePtr<T>(new T(cor::forward<Args>(args)...));
	}

	template< class T >
	constexpr UniquePtr<T> makeUnique(cor::size_t size) {
		return UniquePtr<T>(new cor::RemoveExtent_T<T>[size]);
	}

	template<class T, class D>
	void swap(UniquePtr<T, D>& lhs, UniquePtr<T, D>& rhs) noexcept
	{
		lhs.swap(rhs);
	}
} // !namespace cor

#endif // !UNIQUEPTR_HPP