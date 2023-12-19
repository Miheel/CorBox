#ifndef SORT_HPP
#define SORT_HPP
#include <queue>

#include "utility.hpp"
#include "iter.hpp"

template<class Iter>
void radix(Iter first, Iter last) {



}

template<class Iter>
void insertionSort(Iter first, Iter last) {
	for (auto it = first + 1; it != last; it++)
	{
		auto val = *it;
		auto jt = it;
		for (; (jt != first) && (*(jt - 1) > val); jt--)
		{
			*jt = *(jt - 1);
		}
		*jt = val;
	}
}

template<class Iter>
void merg(Iter first, Iter mid, Iter last) {
	std::vector<int> left(first, mid), right(mid, last);
	auto l = left.begin();
	auto r = right.begin();
	int i = 0;
	while (l != left.end() && r != right.end())
	{
		auto val = *l <= *r ? l++ : r++;
		first[i++] = *val;
	}

	while (l != left.end())
	{
		first[i++] = *l++;
	}
	while (r != right.end())
	{
		first[i++] = *r++;
	}

	//std::vector<int> left(first, mid);
	//auto l = left.begin();
	//auto r = mid;
	//auto dest = first;
	//int i = 0;
	//while (l != left.end() && r != last)
	//{
	//	auto val = *l <= *r ? std::exchange(*l++, *dest) : std::exchange(*r++, *dest);
	//	*dest++ = val;
	//}
	//while (l != left.end())
	//{
	//	*dest++ = *l++;
	//}
}

template<class Iter>
void mergInplace(Iter first, Iter mid, Iter last) {

}

template<class Iter>
void mergsort(Iter first, Iter last) {
	if (last - first > 1) {
		auto mid = std::next(first, (last - first) / 2);
		mergsort(first, mid);
		mergsort(mid, last);
		merg(first, mid, last);
	}
}

template<typename RandomIt>
RandomIt medianOfThree(RandomIt first, RandomIt last)
{
	auto mid = first + (cor::distance(first, last) / 2);

	if (*first > *last) cor::iterSwap(first, last);

	if (*first > *mid) cor::iterSwap(first, mid);

	if (*mid > *last) cor::iterSwap(mid, last);

	cor::iterSwap(mid, last);
	return last;
}

template<typename ForwIt, typename Comp>
ForwIt lomutoPartition(ForwIt first, ForwIt last, Comp comp)
{
	if (first == last) return first;

	//auto pivot = std::prev(last);
	//auto pivot = (last += -1);
	auto pivot = --last;
	auto low = first;
	for (auto next = first; next <= pivot; next++)
	{
		if (comp(*next))
		{
			cor::iterSwap(low, next);
			++low;
		}
	}
	cor::iterSwap(low, pivot);
	return low;
}

template<typename RandIt>
void quickSortLeft(RandIt first, RandIt last)
{
	auto low = first;
	auto high = last;
	while (low < high)
	{
		//auto pivot = *cor::prev(high);
		auto pivot = *medianOfThree(low, cor::prev(high));
		auto middle = lomutoPartition(low, high, [pivot](const auto& it) { return it < pivot; });

		//sort only the smaller of the partiotions first
		//tail recurtion auxillary space gose from O(n) to O(logn) in worst case

		if (cor::distance(low, middle) < cor::distance(middle, high))
		{
			quickSortLeft(low, middle);
			low = middle + 1;
		}
		else {
			quickSortLeft(cor::next(middle), high);
			high = middle;
		}
	}
}

#endif // !SORT_HPP
