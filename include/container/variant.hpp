#ifndef VARIANT_HPP
#define VARIANT_HPP

#include "type_traits.hpp"
namespace cor
{

	template <class... Types>
	class Variant
	{
	public:
		template <class T>
		Variant(T &&value)
		{
			constexpr bool isin = is_present_v<T, Types...>;

			this->idx = index_of_v<T, Types...>;

			std::cout << std::boolalpha << isin << ": " << index() << "\n";

			new (&storage) T(std::forward<T>(value));
		}

		template <class T>
		constexpr Variant &operator=(T &&value) noexcept
		{
			if (is_present_v<T, Types...>)
			{
				idx = index_of_v<T, Types...>;

				new (&storage) T(std::forward<T>(value));
			}

			return *this;
		}
		~Variant() {}

		constexpr size_t index() const noexcept { return idx; }

		alignedUnion_t<0, Types...> &Data() noexcept
		{
			return storage;
		}

	private:
		size_t idx = -1;
		alignedUnion_t<0, Types...> storage;
	};

	template <class T, class... Types>
	constexpr T &get(Variant<Types...> &v)
	{
		if (index_of_v<T, Types...> == v.index())
		{
			return *reinterpret_cast<T *>(&v.Data());
		}
	}

	template <size_t Idx, class... Types>
	constexpr decltype(auto) get(Variant<Types...> &v)
	{
		using type = TypeAt_t<Idx, Types...>;
		return get<type>(v);
	}

}

#endif // !VARIANT_HPP
