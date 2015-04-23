#pragma once
#include <utility>		// for std::forward, std::move
#include "aligned-union.hh"
#include "utils.hh"
#include "const-helper.macro.hh"

namespace meta
{
	struct bad_cast {};

	template <class ...MEMBERS>
	struct variant
	{
		variant() { construct<nil>(); }
		variant(variant const& x) { x.visit<copy_constructor>({*this}); }
		variant(variant     && x) { x.visit<move_constructor>({*this}); }
		~variant() { destruct(); }

		template <class T, class ...TS>
		void emplace(TS&&... xs)
		{
			destruct();
			construct<T>(std::forward<TS>(xs)...);
		}

		template <class T>
		auto is() const noexcept { return (index_of<T> == ti); }

		operator bool () const { return !is<nil>(); }

		CONST_HELPER(
			template <class F>
			decltype(auto) visit(F const& f={}) CONST
			{
				if (!ti) throw bad_cast{};
				return visit<F, MEMBERS...>(f, ti-1);
			}
		);

		CONST_HELPER(
			template <class T> auto& strict_get() CONST
			{
				if (is<T>()) return as<T>();
				throw bad_cast{};
			}
		);

		auto& operator = (variant const& x) { destruct(); x.visit<copy_constructor>({*this}); return *this; }
		auto& operator = (variant     && x) { destruct(); x.visit<move_constructor>({*this}); return *this; }

	private:
		using type_index = utils::index_type;
		struct nil {};

		type_index ti;
		aligned_union<nil, MEMBERS...> au;

		template <class T>
		static constexpr auto index_of = utils::index_of<T, nil, MEMBERS...>;

		CONST_HELPER(
			template <class T> auto& as() CONST noexcept { return au.template as<T>(); }
		);

		CONST_HELPER(
			template <class F, class T>
			decltype(auto) visit(F const& f, type_index) CONST { return f(as<T>()); }
		);

		CONST_HELPER(
			template <class F, class U, class T, class ...TS>
			decltype(auto) visit(F const& f, type_index i) CONST
			{
				if (i) return visit<F, T, TS...>(f, i-1);
				return f(as<U>());
			}
		);


		struct destructor
		{
			template <class T>
			void operator () (T& x) const { x.~T(); }
		};
		void destruct() { visit<destructor>(); }

		template <class T, class ...TS>
		void construct(TS&&... xs)
		{
			au.template construct<T>(std::forward<TS>(xs)...);
			ti = index_of<T>;
		}

		struct move_constructor
		{
			variant& self;
			move_constructor(variant& self) : self{self} {}

			template <class T>
			void operator () (T& x) const { self.construct<T>(std::move(x)); }
		};

		struct copy_constructor
		{
			variant& self;
			copy_constructor(variant& self) : self{self} {}

			template <class T>
			void operator () (T const& x) const { self.construct<T>(x); }
		};
	};
}

#include "const-helper.undef.hh"

