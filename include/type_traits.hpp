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


	template <typename T, typename D>
	struct addPointer
	{
		typedef T* Type;
	};
	template<typename T, typename D>
	using addPointer_T = typename addPointer<T, D>::Type;


	template<class T>
	struct RemoveExtent
	{
		typedef T Type;
	};
	template<class T>
	struct RemoveExtent<T[]>
	{
		typedef T Type;
	};
	template<class T, size_t N>
	struct RemoveExtent<T[N]>
	{
		typedef T Type;
	};
	template<typename T, typename D>
	using RemoveExtent_T = typename RemoveExtent<T>::Type;
} // !namespace cor

#endif // !TYPE_TRAITS_HPP
