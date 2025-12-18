#ifndef UNIQUEPTR_HPP
#define UNIQUEPTR_HPP

#include "utility.hpp"
#include <type_traits>
#include "type_traits.hpp"
#include "types.hpp"
#include "memory.hpp"
#include "allocator.hpp"

namespace cor
{

	// non array types
	template <class T, class Deleter = mem::Deleter<T>>
	class UniquePtr
	{
	public:
		using Pointer = T *;

		// Constructors
		UniquePtr() : pointer(nullptr) {}
		UniquePtr(T *p) : pointer(p) {}

		UniquePtr(UniquePtr &&other) noexcept
		{
			this->reset(other.release());
			deleter = cor::forward<Deleter>(other.getDeleter());
		}

		// assigns
		UniquePtr &operator=(UniquePtr &&other) noexcept
		{
			this->reset(other.release());
			deleter = cor::forward<Deleter>(other.getDeleter());
			return *this;
		}
		UniquePtr &operator=(nullType) noexcept
		{
			this->reset();
			return *this;
		}

		// deleted functionality
		UniquePtr(const UniquePtr &other) = delete;
		UniquePtr &operator=(const UniquePtr &other) = delete;

		// Modifiers
		T *release() noexcept
		{
			auto oldPtr = this->pointer;
			this->pointer = nullptr;

			return oldPtr;
		}
		void reset(T *ptr = Pointer()) noexcept
		{
			auto oldPtr = this->pointer;
			this->pointer = ptr;

			if (oldPtr)
			{
				this->getDeleter()(oldPtr);
			}
		}
		void swap(UniquePtr &other) noexcept
		{
			cor::swap(this->pointer, other.pointer);
			cor::swap(this->deleter, other.deleter);
		}

		// Observers
		T *get() const noexcept
		{
			return pointer;
		}
		Deleter &getDeleter()
		{
			return deleter;
		}
		explicit operator bool() const noexcept
		{
			return this->get() != nullptr;
		}

		// dereferences pointer to the managed object
		T *operator->() const noexcept
		{
			return get();
		}
		T operator*() const noexcept
		{
			return *get();
		}

		// Destructor
		~UniquePtr()
		{
			if (get() != nullptr)
			{
				this->getDeleter()(get());
			}
		}

	private:
		T *pointer = nullptr;
		Deleter deleter;
	};

	// array types
	template <class T, class Deleter>
	class UniquePtr<T[], Deleter>
	{
	public:
		using Pointer = T *;

		// Constructors
		UniquePtr() : pointer(nullptr) {}

		template <class U>
		explicit UniquePtr(U p) noexcept : pointer(p) {}

		UniquePtr(UniquePtr &&other) noexcept
		{
			this->reset(other.release());
			deleter = cor::forward<Deleter>(other.getDeleter());
		}

		// assigns
		UniquePtr &operator=(UniquePtr &&other) noexcept
		{
			{
				this->reset(other.release());
				deleter = cor::forward<Deleter>(other.getDeleter());
				return *this;
			}
		}
		UniquePtr &operator=(nullType) noexcept
		{
			this->reset();
			return *this;
		}

		// deleted functionality
		UniquePtr(const UniquePtr &other) = delete;
		UniquePtr &operator=(const UniquePtr &other) = delete;

		// Modifiers
		T *release() noexcept
		{
			auto oldPtr = this->pointer;
			this->pointer = nullptr;

			return oldPtr;
		}
		template <class U>
		void reset(U ptr) noexcept
		{
			auto oldPtr = this->pointer;
			this->pointer = ptr;

			if (oldPtr)
			{
				this->getDeleter()(oldPtr);
			}
		}
		void reset(cor::nullType = nullptr) noexcept
		{
			reset(Pointer());
		}
		void swap(UniquePtr &other) noexcept
		{
			cor::swap(this->pointer, other.pointer);
			cor::swap(this->deleter, other.deleter);
		}

		// Observers
		T *get() const noexcept
		{
			return pointer;
		}
		Deleter &getDeleter()
		{
			return deleter;
		}
		explicit operator bool() const noexcept
		{
			return this->get() != nullptr;
		}

		// dereferences pointer to the managed object
		T *operator->() const noexcept
		{
			return pointer;
		}
		const T operator*() const noexcept
		{
			return *get();
		}

		T &operator[](cor::usize i) const
		{
			return this->get()[i];
		}

		// Destructor
		~UniquePtr()
		{
			if (get() != nullptr)
			{
				this->getDeleter()(get());
			}
		}

	private:
		T *pointer;
		Deleter deleter;
	};

	// non array type
	template <class T, class... Args, cor::EnableIf_T<!cor::IsArray_T<T>, bool> = true>
	constexpr UniquePtr<T> makeUnique(Args &&...args)
	{
		return UniquePtr<T>(mem::allocate<T>(args...));
	}

	// array type
	template <class T, cor::EnableIf_T<cor::IsArray_T<T> && std::extent_v<T> == 0, bool> = true>
	constexpr UniquePtr<T> makeUnique(cor::usize size)
	{
		return UniquePtr<T>(mem::allocate_r_extent<T>(size));
	}

	template <class T, class D>
	void swap(UniquePtr<T, D> &lhs, UniquePtr<T, D> &rhs) noexcept
	{
		lhs.swap(rhs);
	}

	template <class T, class D>
	bool operator==(const UniquePtr<T, D> &ptr, nullType) noexcept
	{
		return !ptr;
	}

	template <class T, class D>
	bool operator!=(const UniquePtr<T, D> &ptr, nullType) noexcept
	{
		return (bool)ptr;
	}

} // !namespace cor

#endif // !UNIQUEPTR_HPP
