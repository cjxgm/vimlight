#pragma once
#include "variant.hh"
#include "const-helper.macro.hh"
#include <utility>		// for std::forward

namespace meta
{
	template <class T>
	struct optional
	{
		using self = optional;
		optional() = default;

		template <class = utils::disable_if_base_of<self, T>>
		optional(T&& x) : v{std::forward<T>(x)} {}

		operator bool () const { return v; }

		CONST_HELPER(
			auto& get() CONST { return v.template strict_get<T>(); }
		);

		CONST_HELPER(
			operator auto& () CONST { return get(); }
		);

	private:
		using variant_type = variant<T>;
		variant_type v;
	};
}

#include "const-helper.undef.hh"

