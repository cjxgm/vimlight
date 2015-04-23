#pragma once
#include <type_traits>
#include <utility>		// for std::forward
#include "utils.hh"
#include "constraints.hh"
#include "const-helper.macro.hh"

namespace meta
{
	template <class NIL, class ...MEMBERS>
	struct aligned_union : non_transferable
	{
		template <class T>
		static constexpr auto nil_requirement =
				std::is_nothrow_constructible<T>::value &&
				std::is_nothrow_destructible <T>::value;

		using nil = std::decay_t<NIL>;
		static_assert(nil_requirement<nil>, "NIL does not meet the nil-requirement");

		static constexpr auto alignment_value()
		{
			return utils::alignment_of<nil, std::decay_t<MEMBERS>...>;
		}

		static constexpr auto size()
		{
			return utils::max(sizeof(nil), sizeof(std::decay_t<MEMBERS>)...);
		}

		template <class T>
		aligned_union(T&& x) { construct<T>(std::forward<T>(x)); }
		aligned_union() : aligned_union{nil{}} {}

		template <class T, class ...TS>
		void construct(TS&&... xs)
		{
			using type = std::decay_t<T>;
			try {
				new (data) type(std::forward<TS>(xs)...);
			}
			catch (...) {
				new (data) nil;
				throw;
			}
		}

		template <class T>
		void destruct() { data.~T(); }

		CONST_HELPER(
			template <class T>
			auto& as() CONST noexcept
			{
				using type = utils::const_if<decltype(this), std::decay_t<T>>;
				return *reinterpret_cast<type *>(data);
			}
		);

	private:
		alignas(alignment_value()) char data[size()];
	};
}

#include "const-helper.undef.hh"

