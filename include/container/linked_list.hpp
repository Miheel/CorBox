#ifndef LINKED_LIST_HPP
#define LINKED_LIST_HPP

#include <iostream>
#include "utility.hpp"
namespace cor {

	template<typename T>
	class Linked_list
	{
	private:
		struct node_t {
			node_t(T value, node_t * next = nullptr, node_t * prev = nullptr)
				:value(value), next(next), prev(prev)
			{}
			T value;
			node_t * next = nullptr;
			node_t * prev = nullptr;
		};
		using pointer = node_t * ;
		using const_pointer = const node_t*;
		using reference = T & ;
		using const_reference = const T&;


	public:
		Linked_list()
			:head(nullptr), tail(nullptr), nrOfNodes(0)
		{}
		Linked_list(const Linked_list & src) { this->copy(src); }
		Linked_list(Linked_list && src);
		~Linked_list() {
			while (!this->is_empty())
			{
				this->pop_back();
			}
			this->nrOfNodes = 0;
		}

		Linked_list<T>& operator=(const Linked_list & rhs);
		Linked_list<T>& operator=(Linked_list&& rhs);

		// adds elements to the back
		Linked_list<T> &operator+=(const Linked_list & rhs);

		// inserting elements
		void insert(T value, size_t pos);
		void push_front(T value);
		void push_back(T value);
		void add_node(pointer node);

		//Iterators
		pointer begin() { return this->head; }
		pointer end() { return this->tail; }
		const_pointer begin() const { return this->head; }
		const_pointer end() const { return this->tail; }
		pointer operator[](size_t pos) const {
			pointer foundValue = 0;
			node_t *walker = head;

			if (pos == 0)
			{
				foundValue = this->head;
			}
			else
			{
				for (size_t i = 0; i <= pos; i++, walker = walker->next)
				{
					foundValue = walker;
				}
			}

			return foundValue;
		}

		// accessing elements
		const_reference front() const { return this->head->value; }
		const_reference back() const { return this->tail->value; }
		reference front() { return this->head->value; }
		reference back() { return this->tail->value; }
		T at(size_t pos) const;

		// removing elements
		void remove(size_t pos);
		void pop_front();
		void pop_back();

		// informational
		size_t size() const { return this->nrOfNodes; }
		bool is_empty() const { return this->nrOfNodes == 0 ? true : false; }
		bool is_order() const;

		// output
		void print() const;
		void print_reverse() const;

		Linked_list<T> &merge(Linked_list &list1, Linked_list &list2);
		void merge(Linked_list &other);

		void swap(Linked_list &other) noexcept;

	private:

		node_t * head;
		node_t * tail;
		size_t nrOfNodes;

		void copy(Linked_list const &other) {
			this->nrOfNodes = 0;
			if (!other.is_empty())
			{
				node_t* walker = other.head->next;
				node_t* current = this->head = new node_t(other.head->value);
				this->nrOfNodes++;

				while (walker != nullptr)
				{
					node_t* next = new node_t(walker->value);

					current->next = next;
					next->prev = current;

					current = current->next;
					walker = walker->next;
					this->nrOfNodes++;
				}
				this->tail = current;
			}
		}
	};

	template<typename T>
	inline Linked_list<T>::Linked_list(Linked_list && src)
	{
		this->swap(src);
	}

	template<typename T>
	inline Linked_list<T>& Linked_list<T>::operator=(const Linked_list & rhs)
	{
		if (this != &rhs)
		{
			while (!this->is_empty())
			{
				this->pop_back();
			}
			this->copy(rhs);
		}

		return *this;
	}

	template<typename T>
	inline Linked_list<T> & Linked_list<T>::operator=(Linked_list && rhs)
	{
		this->swap(rhs);
		return *this;
	}

	template<typename T>
	inline Linked_list<T>& Linked_list<T>::operator+=(const Linked_list & rhs)
	{
		node_t *walker = rhs.head;

		while (walker != nullptr)
		{
			this->push_back(walker->value);
			walker = walker->next;
		}
		return *this;
	}

	template<typename T>
	inline void Linked_list<T>::insert(T value, size_t pos)
	{
		node_t *walker = head;
		walker = this->head;

		if (pos == 0)
		{
			this->push_front(value);
		}
		else if (pos == this->size())
		{
			this->push_back(value);
		}
		else
		{
			for (size_t i = 0; i <= pos && walker != nullptr; i++)
			{
				if (i == pos)
				{
					node_t *newNode = new node_t(value, walker, walker->prev);

					node_t *ptrPrev = walker->prev;
					ptrPrev->next = newNode;
					walker->prev = newNode;
					this->nrOfNodes++;

				}
				walker = walker->next;
			}
		}
	}

	template<typename T>
	inline void Linked_list<T>::push_front(T value)
	{
		node_t *tmpNode = new node_t(value, this->head, nullptr);

		if (head == nullptr)
		{
			this->head = this->tail = tmpNode;
		}
		else
		{
			this->head->prev = tmpNode;
			this->head = tmpNode;
		}

		this->nrOfNodes++;
	}

	template<typename T>
	inline void Linked_list<T>::push_back(T value)
	{
		node_t *tmpNode = new node_t(value, nullptr, this->tail);

		if (this->tail == nullptr)
		{
			this->head = this->tail = tmpNode;
		}
		else
		{
			this->tail->next = tmpNode;
			this->tail = tmpNode;
		}

		this->nrOfNodes++;
	}

	template<typename T>
	inline void Linked_list<T>::add_node(pointer node)
	{
		if (this->head == nullptr)
		{
			this->head = this->tail = node;
		}
		else
		{
			this->tail->next = node;
		}
		this->nrOfNodes++;
	}

	template<typename T>
	inline T Linked_list<T>::at(size_t pos) const
	{
		T foundValue = 0;
		node_t *walker = head;

		if (pos == 0)
		{
			foundValue = this->head->value;
		}
		else
		{
			for (size_t i = 0; i <= pos; i++, walker = walker->next)
			{
				foundValue = walker->value;
			}
		}

		return foundValue;
	}

	template<typename T>
	inline void Linked_list<T>::remove(size_t pos)
	{
		node_t *walker = head;
		walker = this->head;

		if (pos == 0)
		{
			this->pop_front();
		}
		else if (pos == this->size() - 1)
		{
			this->pop_back();
		}
		else
		{
			for (size_t i = 0; i <= pos && walker != nullptr; i++)
			{
				if (i == pos)
				{
					walker->prev->next = walker->next;
					walker->next->prev = walker->prev;
					walker->next = nullptr;
					walker->prev = nullptr;
					delete walker;
					this->nrOfNodes--;
				}
				else
				{
					walker = walker->next;
				}
			}
		}
	}

	template<typename T>
	inline void Linked_list<T>::pop_front()
	{
		node_t *tmpHead = this->head;

		if (this->head->next != nullptr)
		{
			this->head = this->head->next;
			this->head->prev = nullptr;
		}
		else
		{
			this->head = this->tail = nullptr;
		}

		delete tmpHead;
		this->nrOfNodes--;
	}

	template<typename T>
	inline void Linked_list<T>::pop_back()
	{
		node_t *tmpTail = this->tail;

		if (this->tail->prev != nullptr)
		{
			this->tail = this->tail->prev;
			this->tail->next = nullptr;
		}
		else
		{
			this->head = this->tail = nullptr;
		}

		delete tmpTail;
		this->nrOfNodes--;
	}

	template<typename T>
	inline bool Linked_list<T>::is_order() const
	{
		node_t* walker = this->head, *walker_next = this->head->next;
		while (walker_next != nullptr)
		{
			if (walker->value <= walker_next->value)
			{
				walker = walker_next;
				walker_next = walker_next->next;
			}
			else
			{
				return false;
			}
		}
		return true;
	}

	template<typename T>
	inline void Linked_list<T>::print() const
	{
		if (!this->is_empty())
		{
			int i = 0;
			for (node_t *walker = head; walker != nullptr; walker = walker->next, i++)
			{
				std::cout << walker->value << ", ";
			}
		}
		else
		{
			std::cout << "list is empty\n";
		}
	}

	template<typename T>
	inline void Linked_list<T>::print_reverse() const
	{
		if (!this->is_empty())
		{
			for (node_t *walker = tail; walker != nullptr; walker = walker->prev)
			{
				std::cout << walker->value << ", ";
			}
		}
		else
		{
			std::cout << "list is empty\n";
		}
	}

	template<typename T>
	inline Linked_list<T>& Linked_list<T>::merge(Linked_list &list1, Linked_list &list2)
	{

		while (!list1.is_empty() && !list2.is_empty())
		{
			if (list1.front() < list2.front())
			{
				this->push_back(list1.front());
				list1.pop_front();
			}
			else
			{
				this->push_back(list2.front());
				list2.pop_front();
			}
		}
		while (!list1.is_empty())
		{
			this->push_back(list1.front());
			list2.pop_front();
		}
		while (!list2.is_empty())
		{
			this->push_back(list2.front());
			list2.pop_front();
		}
		return *this;
	}

	template<typename T>
	inline void Linked_list<T>::merge(Linked_list & other)
	{
		/*	if (!(*this == other))
			{
				node_t* walker_this = this->head, *walker_other = other.head, *temp_this = nullptr, *temp_other = nullptr;

				while (walker_this != nullptr && walker_other != nullptr)
				{
					if (walker_this->value < other.front())
					{
						walker_this = walker_this->next;
					}
					else
					{
						temp_other = walker_other->next;

						if (walker_this->prev !=)
						{
							walker_this->prev->next = walker_other;
						}
						else
						{
							walker_this->prev = walker_other
						}


						walker_other->next = walker_this;
						walker_other->prev = walker_this->prev;
						walker_this->prev = walker_other;
						temp_this->next = walker_other;

						walker_other = temp_other;
						other.head = walker_other;
						this->nrOfNodes++;
					}
				}
				if (!other.is_empty())
				{
					walker_this = walker_other;
					walker_other->prev = walker_this;
					this->tail->prev = walker_other;
					this->nrOfNodes++;
				}
			}*/
	}

	template<typename T>
	inline void Linked_list<T>::swap(Linked_list & other) noexcept
	{
		cor::swap(this->head, other.head);
		cor::swap(this->tail, other.tail);
		cor::swap(this->nrOfNodes, other.nrOfNodes);
	}

	template< class T>
	bool operator==(const Linked_list<T>& lhs, const Linked_list<T>& rhs) {
		bool is_equal = false;
		if (lhs.size() == rhs.size())
		{
			auto walker_l = lhs.begin(), walker_r = rhs.begin();

			for (size_t i = 0; i < lhs.size(); i++)
			{
				if (walker_l->value == walker_r->value)
				{
					walker_l = walker_l->next;
					walker_r = walker_r->next;
					is_equal = true;
				}
				else
				{
					is_equal = false;
					break;
				}
			}

		}
		return is_equal;
	}

} // !namespace cor

#endif // !LINKED_LIST_HPP
