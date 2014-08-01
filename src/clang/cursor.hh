#pragma once
#include "c.hh"
#include "internal.hh"
#include "range.hh"
#include "string.hh"
#include <string>
#include <functional>

namespace clang
{
	struct cursor : public internal::bin<c::cursor::type>
	{
		using self_type = cursor;
		using super_type = internal::bin<c::cursor::type>;
		using name_type = std::string;
		using visitor_type = std::function<bool(const self_type&)>;




		struct kind : public internal::bin<c::cursor::kind::type>
		{
			using super_type = internal::bin<c::cursor::kind::type>;

			kind(value_type value) : super_type(value) {}

			name_type name() const { return clang::string{c::cursor::kind::get_spelling(get())}; }
			bool is_decl() const { return c::cursor::kind::is_declaration(get()); }
			bool is_ref () const { return c::cursor::kind::is_reference  (get()); }
		};




		cursor(value_type value) : super_type(value) {}

		range range() const { return c::cursor::get_extent(get()); }
		kind kind() const { return c::cursor::get_kind(get()); }
		name_type name() const { return clang::string{c::cursor::get_spelling(get())}; }

		void each_child(const visitor_type& visitor) const
		{
			c::cursor::childs::visit(get(), &raw_visitor,
					const_cast<visitor_type*>(&visitor));
		}

	private:
		using visit_result_type = c::cursor::childs::visit_result::type;
		using raw_cursor_type = c::cursor::type;
		static visit_result_type raw_visitor(
				raw_cursor_type cursor,
				raw_cursor_type parent,
				void* data)
		{
			clang::cursor cu(cursor);
			auto visitor = reinterpret_cast<const visitor_type*>(data);
			const visitor_type& visit = *visitor;
			return (visit(cu) ?
					c::cursor::childs::visit_result::into :
					c::cursor::childs::visit_result::next);
		}
	};
};

