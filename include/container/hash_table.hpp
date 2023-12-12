#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP

#include "uniquePtr.hpp"
#include "array.hpp"

class HashTable
{
private:
	struct Node
	{
		Node(int data) :
			data(data) {}

		int data;
		cor::UniquePtr<Node> next = nullptr;
	};

	cor::Array<cor::UniquePtr<Node>> hashTable;
	size_t size = 0;
	const float LOAD_FACTOR = 0.75;


	template<typename T>
	int hashFunk(T key)
	{
		return key % this->hashTable.size();
	}

public:
	HashTable();

	HashTable(size_t size) :hashTable(size)
	{}

	template<typename RandomIt>
	HashTable(RandomIt first, RandomIt last, size_t size)
	{
		auto newSize = std::floor(size / 0.5);
		this->hashTable.resize(int(newSize));
		for (auto it = first; it != last; it++)
		{
			this->insert(*it);
		}
	}

	HashTable(const HashTable& other) {}
	HashTable(HashTable&& other) {}

	template <typename InputIt>
	HashTable(InputIt first, InputIt last);

	HashTable& operator=(const HashTable& other);
	HashTable& operator=(HashTable&& other);

	template<typename T>
	Node* search(T key)
	{
		int index = this->hashFunk(key);

		auto node = this->hashTable.at(index).get();

		while (node != nullptr)
		{
			if (node->data == key)
			{
				return node;
			}
			node = node->next.get();
		}

		return nullptr;
	}

	template<typename T>
	void insert(T key)
	{
		auto newNode = cor::makeUnique<Node>(key);

		int index = this->hashFunk(key);

		if (this->hashTable[index] == nullptr)
		{
			this->hashTable[index] = cor::isMovable(newNode);
			size++;
		}
		else
		{
			auto curr = this->hashTable[index].get();
			while (curr->next != nullptr)
			{
				curr = curr->next.get();
			}
			curr->next = cor::isMovable(newNode);
			size++;
		}

		if (loadFactor() > LOAD_FACTOR)
		{
			this->resizeAndRehash(size * 2);
		}
	}

	template <typename T>
	void remove(T key) {
		if (search(key))
		{
			int index = this->hashFunk(key);

			if (this->hashTable.at(index).get()->data == key)
			{
				this->hashTable.at(index) = cor::isMovable(this->hashTable.at(index).get()->next);
			}
			else
			{
				auto head = this->hashTable.at(index).get();
				while (head->next && head->next->data != key)
				{
					head = head->next.get();
				}
				if (head->next) {
					head->next = cor::isMovable(head->next->next);
				}
			}

		}
	}

	void resizeAndRehash(size_t newSize){
		HashTable newTable(newSize);

		for (auto &e : hashTable)
		{
			if (e)
			{
				auto head = e.get();
				while (head) {
					newTable.insert(head->data);
					head = head->next.get();
				}
			}

		}
		this->swap(newTable);
	}

	float loadFactor() const { return (float)this->size / this->hashTable.size(); }

	void swap(HashTable& other) {
		this->hashTable.swap(other.hashTable);
		cor::swap(this->size, other.size);
	}
	void print()
	{
		//for (auto &node : this->hashTable)
		//{
		//	if (node == nullptr)
		//	{
		//		std::cout << "null\n";
		//	}
		//	else if (node->next == nullptr)
		//	{
		//		std::cout << node->data << "-> null\n";
		//	}
		//	else {

		//		while (node->next != nullptr)
		//		{
		//			std::cout << node->data << " -> ";
		//			node = node->next;
		//		}
		//		std::cout << node->data << "-> null\n";
		//	}
		//}
	}
};

#endif // !HASH_TABLE_HPP