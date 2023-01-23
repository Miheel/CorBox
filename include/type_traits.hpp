#ifndef TYPE_TRAITS_HPP
#define TYPE_TRAITS_HPP

namespace cor {

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

	template<typename T>
	using RemoveReference_T = typename RemoveReference<T>::Type;

} // !namespace cor

#endif // !TYPE_TRAITS_HPP
