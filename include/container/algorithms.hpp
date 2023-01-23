#ifndef ALGORITHMS_HPP
#define ALGORITHMS_HPP

namespace cor {

	template <typename T>
	void swap(T& first, T& second) {
		auto temp = first;
		first = second;
		second = temp;
	}

	template <typename Iter>
	void iterSwap(Iter first, Iter second) {
		cor::swap(*first, *second);
	}

	template<typename InIt, typename OutIt, typename Func>
	inline OutIt transform(const InIt first, const InIt last, OutIt dest, Func func) {
		for (auto tempFirst = first; tempFirst != last; tempFirst++, dest++)
		{
			*dest = func(*tempFirst);
		}
		return dest;
	}

	template<typename T>
	bool cycleDetect(const T list) {

		if (list->begin() == list->end())
		{
			return false;
		}

		auto tortoise = list->begin();
		auto hare = list->begin()->next;

		while (hare != tortoise && hare != nullptr)
		{
			tortoise = tortoise->next;
			hare = hare->next->next;
		}
		if (hare == tortoise)
		{
			return true;
		}
		return false;
	}

} // !namespace cor

#endif // !ALGORITHMS_HPP
