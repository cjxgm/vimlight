#pragma once
#include <type_traits>

namespace meta
{
	namespace utils
	{
		//------ left fold
		//
		//	foldl(F, x, xs...)		=> left fold x:xs by F
		//
		namespace foldl_impl
		{
			template <class F, class U>
			constexpr auto foldl(F, U y) { return y; }

			template <class F, class U, class T, class ...TS>
			constexpr auto foldl(F f, U y, T x, TS... xs) { return foldl(f, f(y, x), xs...); }
		}

		using foldl_impl::foldl;




		//------ max
		//
		//	max(x, xs...)			=> maximum value of x:xs
		//
		namespace max_impl
		{
			template <class T>
			constexpr auto max(T a, T b) { return (a > b ? a : b); }
		}

		template <class T, class ...TS>
		constexpr auto max(T x, TS ...xs)
		{
			return foldl(max_impl::max<T>, x, xs...);
		}




		//------ alignment of
		//
		//	alignment_of<TS...>		=> maximum alignment of TS
		//
		template <class ...TS>
		constexpr auto alignment_of = max(alignof(TS)...);




		//------ index of
		//
		//	index_of<U, TS...>		=> index of U in TS
		//
		namespace index_of_impl
		{
			using index_type = decltype(sizeof 0); // same as std::size_t according to the standard

			template <index_type I, class U, class ...TS>
			constexpr auto index_of = I;

			template <index_type I, class U, class T, class ...TS>
			constexpr auto index_of<I, U, T, TS...> = index_of<I+1, U, TS...>;

			template <index_type I, class U, class ...TS>
			constexpr auto index_of<I, U, U, TS...> = I;

			template <index_type I, class ...TS>
			constexpr auto check()
			{
				static_assert(I < sizeof...(TS), "not one of the specified types");
				return I;
			};

			template <class U, class ...TS>
			constexpr auto checked = check<index_of<0, U, TS...>, TS...>();
		}

		template <class U, class ...TS>
		constexpr auto index_of = index_of_impl::checked<
				std::decay_t<U>,
				std::decay_t<TS>...
		>;

		using index_of_impl::index_type;




		//------ const if
		//
		//	const_if<U, T>				=> T
		//	const_if<U const*, T>		=> const T
		//
		namespace const_if_impl
		{
			template <class U, class T> struct const_if              { using type =       T; };
			template <class U, class T> struct const_if<U const*, T> { using type = const T; };
		}

		template <class U, class T>
		using const_if = typename const_if_impl::const_if<U, T>::type;
	}
}

