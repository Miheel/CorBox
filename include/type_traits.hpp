#ifndef TYPE_TRAITS_HPP
#define TYPE_TRAITS_HPP

#include "algorithms.hpp"
#include "types.hpp"

namespace cor
{

	template <typename T>
	struct RemoveReference
	{
		typedef T Type;
	};

	template <typename T>
	struct RemoveReference<T&>
	{
		typedef T Type;
	};

	template <typename T>
	struct RemoveReference<T&&>
	{
		typedef T Type;
	};

	template <typename T>
	using RemoveReference_T = typename RemoveReference<T>::Type;

	template <typename T, typename D>
	struct addPointer
	{
		typedef T* Type;
	};

	template <typename T, typename D>
	using addPointer_T = typename addPointer<T, D>::Type;

	template <class T>
	struct RemoveExtent
	{
		typedef T Type;
	};

	template <class T>
	struct RemoveExtent<T[]>
	{
		typedef T Type;
	};

	template <class T, usize N>
	struct RemoveExtent<T[N]>
	{
		typedef T Type;
	};

	template <typename T>
	using RemoveExtent_T = typename RemoveExtent<T>::Type;

	template <bool B, class T = void>
	struct EnableIf
	{
	};

	template <class T>
	struct EnableIf<true, T>
	{
		typedef T Type;
	};

	template <bool B, class T = void>
	using EnableIf_T = typename EnableIf<B, T>::Type;

	template <class T, T val>
	struct IntegralConstant
	{
		typedef T value_type;
		typedef IntegralConstant type;

		constexpr static T value = val;

		constexpr operator value_type() const noexcept
		{
			return value;
		}

		constexpr value_type operator()() const noexcept
		{
			return value;
		}
	};

	template <bool B>
	using BoolConstant = IntegralConstant<bool, B>;

	using TrueType = BoolConstant<true>;
	using FalseType = BoolConstant<false>;

	template <class T>
	struct IsIntegral
		: FalseType
	{
	};
	template <>
	struct IsIntegral<int>
		: TrueType
	{
	};
	template <>
	struct IsIntegral<long long>
		: TrueType
	{
	};

	template <class T>
	inline constexpr bool IsIntegral_v = IsIntegral<T>::value;

	template <class T>
	struct IsArray : FalseType
	{
	};

	template <class T>
	struct IsArray<T[]> : TrueType
	{
	};

	template <class T, usize N>
	struct IsArray<T[N]> : TrueType
	{
	};

	template <class T>
	inline constexpr bool IsArray_T = IsArray<T>::value;

	template<class T>
	struct IsPointer :FalseType
	{};

	template<class T>
	struct IsPointer<T*> :TrueType
	{};

	template<class T>
	struct IsPointer<const T*> :TrueType
	{};

	template<class T>
	struct IsPointer<T* const> :TrueType
	{};

	template<class T>
	inline constexpr bool IsPtr_V = IsPointer<T>::value;

	template<class T, class ...Types>
	struct is_present
	{
		inline static constexpr bool value = (std::is_same_v<T, Types> || ...);
	};

	template<class T, class ...Types>
	inline constexpr bool is_present_v = is_present<T, Types...>::value;


	template<class T, class...Rest>
	struct index_of
	{};

	template<class T, class...Rest>
	struct index_of<T, T, Rest...>
	{
		static constexpr size_t value = 0;
	};

	template<class T, class U, class...Rest>
	struct index_of<T, U, Rest...>
	{
		static constexpr size_t value = std::is_same_v<T, U> ? 0 : 1 + index_of<T, Rest...>::value;
	};

	template<class T, class ...Rest>
	static constexpr size_t index_of_v = index_of<T, Rest...>::value;

	template <size_t Index, typename... Types>
	struct TypeAt;

	template<size_t Idx, class First, class...Rest>
	struct TypeAt<Idx, First, Rest...>
	{
		using type = typename TypeAt<Idx - 1, Rest...>::type;
	};

	template<class First, class...Rest>
	struct TypeAt<0, First, Rest...>
	{
		using type = First;
	};

	template <size_t Index, typename... Types>
	using TypeAt_t = typename TypeAt<Index, Types...>::type;

	template<size_t Len, class ...Types>
	struct alignedUnion
	{
		static constexpr size_t aligned_value = max_of({ alignof(Types)... });
		static constexpr size_t size_value = max_of({ Len, sizeof(Types)... });
		struct Type
		{
			alignas(aligned_value) char ch[size_value];
		};
	};

	template<size_t Len, class ...Types>
	using alignedUnion_t = typename alignedUnion<Len, Types...>::Type;

} // !namespace cor

#endif // !TYPE_TRAITS_HPP
