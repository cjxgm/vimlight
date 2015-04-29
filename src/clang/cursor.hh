#pragma once
#include "c.hh"
#include "resource.hh"
#include "range.hh"
#include "location.hh"
#include "string.hh"
#include "../meta/optional.hh"
#include <string>
#include <functional>
#include <memory>


namespace clang
{
	struct cursor : public resource::bin<c::cursor::type>
	{
		using self_type = cursor;
		using super_type = bin;
		using name_type = std::string;
		using identifier_type = std::string;
		using visitor_type = std::function<bool(self_type const&)>;




		struct kind : public resource::bin<c::cursor::kind::type>
		{
			using super_type = bin;

			kind(value_type value) : super_type(value) {}

			name_type name() const { return clang::string{c::cursor::kind::get_spelling(get())}; }
			bool is_decl() const { return c::cursor::kind::is_declaration(get()); }
			bool is_ref () const { return c::cursor::kind::is_reference  (get()); }
		};




		cursor(value_type value) : super_type(value) {}

		range range() const { return c::cursor::get_extent(get()); }
		location location() const { return c::cursor::get_location(get()); }
		kind kind() const { return c::cursor::get_kind(get()); }
		name_type name() const { return clang::string{c::cursor::get_spelling(get())}; }
		cursor reference() const { return clang::cursor{c::cursor::get_referenced((get()))}; }
		auto first_child() const
		{
			using optional_cursor = meta::optional<cursor>;
			optional_cursor oc;
			c::cursor::childs::visit(get(),
					[] (auto cs, auto, auto data) {
						auto& oc = *reinterpret_cast<optional_cursor*>(data);
						oc = cursor{cs};
						return c::cursor::childs::visit_result::stop;
					}, &oc);
			return oc;
		}

		identifier_type identifier() const
		{
			using std::to_string;
			intptr_t data[] = {
				reinterpret_cast<intptr_t>(get().data[0]),
				reinterpret_cast<intptr_t>(get().data[1]),
				reinterpret_cast<intptr_t>(get().data[2]),
			};
			return	to_string(data[0]) + ":" +
					to_string(data[1]) + ":" +
					to_string(data[2]);
		}

		void each_child(visitor_type const& visitor) const
		{
			c::cursor::childs::visit(get(), &raw_visitor,
					const_cast<visitor_type*>(&visitor));
		}

	private:
		using visit_result_type = c::cursor::childs::visit_result::type;
		using raw_cursor_type = c::cursor::type;
		static visit_result_type raw_visitor(
				raw_cursor_type cursor,
				raw_cursor_type /*parent*/,
				void* data)
		{
			clang::cursor cu(cursor);
			auto visitor = reinterpret_cast<visitor_type const*>(data);
			visitor_type const& visit = *visitor;
			return (visit(cu) ?
					c::cursor::childs::visit_result::into :
					c::cursor::childs::visit_result::next);
		}
	};
}

