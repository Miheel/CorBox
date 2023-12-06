#ifndef SLICE_HPP
#define SLICE_HPP

namespace cor {

	template<class T>
	class Slice {
	public:
		Slice(T* first = nullptr, T* last = nullptr) : first(first), last(last) {}

		size_t size() const { return last - first; }
		bool empty() const { return first == nullptr; }

		T& operator[](size_t index) const { return first[index]; }
		T& at(size_t index) const { return first[index]; }

		T* data() { return first; }

		const T* begin() const { return first; }
		const T* end() const { return last; }

		T* begin() { return first; }
		T* end() { return last; }

	private:
		T* first = nullptr;
		T* last = nullptr;
	};

}
#endif // !SLICE_HPP
