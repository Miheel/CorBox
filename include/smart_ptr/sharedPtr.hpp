#ifndef SHAREDPTR_HPP
#define SHAREDPTR_HPP

#include <memory>
#include <utility>
#include "utility.hpp"
#include "memory.hpp"
#include "type_traits.hpp"
#include "types.hpp"

namespace cor {

	template<class T, class Deleter = mem::Deleter<T>> /*std::default_delete<T>*/
	struct Control_block
	{
	private:
		T* pointer = nullptr;
		Deleter deleter;
		long use_count = 0;
		long weak_count = 0;


	public:
		Control_block() = default;
		Control_block(T* ptr) :pointer(ptr) { use_count++; }
		Control_block(T* ptr, Deleter deleter) :pointer(ptr), deleter(deleter) {}
		Control_block(Deleter deleter) :deleter(deleter) {}

		void decRefCount() { if (use_count > 0)use_count--; }
		void decWeakCount() { if (weak_count > 0)weak_count--; }

		long GetUseCount() { return use_count; }
		long GetWeakCount() { return weak_count; }

		void deleteCPtr() { deleter(pointer); }

		~Control_block() = default;
	};

	template<class T>
	class SharedPtr
	{
	public:
		using Elem_t = std::remove_extent_t<T> /*cor::RemoveExtent_T<T>*/;
		using Elem_T_ptr = Elem_t * ;
		using Elem_t_ref = Elem_t & ;
		using C_block_ptr = Control_block<Elem_t>*;

	private:
		Elem_T_ptr pointer;
		C_block_ptr c_block;

		C_block_ptr GetCblock() { return c_block; }

	public:
		//Constructors

		constexpr SharedPtr() noexcept :pointer(nullptr), c_block(nullptr) {}
		constexpr SharedPtr(std::nullptr_t) noexcept :pointer(nullptr), c_block(nullptr) {}

		template< class Y>
		SharedPtr(Y* ptr) : pointer(ptr), c_block(mem::allocate<Control_block<Y>>(pointer)) {}

		template< class Y, class Deleter >
		SharedPtr(Y* ptr, Deleter d) : pointer(ptr), c_block(ptr, d) {}

		//assigns
		//SharedPtr& operator =(SharedPtr&& other) noexcept {}

		//SharedPtr(const SharedPtr& other);
		//SharedPtr& operator =(const SharedPtr& other);

		//Modifiers
		void reset() noexcept {
			c_block->decRefCount();
			SharedPtr().swap(*this);
		}

		template< class Y >
		void reset(Y* ptr) {
			SharedPtr(ptr).swap(*this);
		}

		template< class Y, class Deleter >
		void reset(Y* ptr, Deleter d) {
			SharedPtr(ptr, d).swap(*this);
		}

		void swap(SharedPtr& other) noexcept {
			cor::swap(this->pointer, other.pointer);
			cor::swap(this->c_block, other.c_block);
		}

		//Observers
		Elem_T_ptr get() const noexcept {
			return pointer;
		}

		//dereferences pointer to the managed object
		Elem_T_ptr operator->() const noexcept {
			return get();
		}
		Elem_t_ref operator*() const noexcept {
			return *get();
		}

		//Array version, unique_ptr<T[]>
		Elem_t_ref operator[](cor::size_t i) const {
			return this->get()[i];
		}

		long useCount() const noexcept {
			if (c_block)
			{
				return c_block->GetUseCount();
			}
			return 0;
		}

		bool unique() const noexcept {
			return this->useCount() == 1;
		}

		explicit operator bool() const noexcept {
			return this->get() != nullptr;
		}

		template< class Y >
		bool ownerBefore(const SharedPtr<Y>& other) {
			return c_block < other.GetCblock();
		}

		//Destructor
		~SharedPtr() {
			if (c_block)
			{
				c_block->decRefCount();
				c_block->decWeakCount();
				if (c_block->GetUseCount() == 0 && c_block->GetWeakCount() == 0)
				{
					c_block->deleteCPtr();
					mem::deallocate<Control_block<Elem_t>>(c_block);
				}
			}
		}
	};

	template<class T, class ...Args>
	constexpr SharedPtr<T> makeShared(Args && ...args)
	{
		void* storage = mem::allocateRaw<mem::align_of<SharedPtr<T>>>(sizeof(SharedPtr<T>) + sizeof(T));
		//void* storage = operator new(sizeof(SharedPtr<T>) + sizeof(T));
		//return SharedPtr<T>(::new (storage) T(std::forward<Args>(args)...));
		return SharedPtr<T>(mem::allocatePlace<T>(storage, args...));
	}


	//template< class T >
	//constexpr SharedPtr<T> makeUnique(cor::size_t size) {
	//	return SharedPtr<T>(new cor::RemoveExtent_T<T>[size]);
	//}

	template<class T>
	void swap(SharedPtr<T>& lhs, SharedPtr<T>& rhs) noexcept
	{
		lhs.swap(rhs);
	}

} // !namespace cor

#endif // !SHAREDPTR_HPP
