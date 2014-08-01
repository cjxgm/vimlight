#pragma once
extern "C"
{
	#include <lua.h>
	#include <lauxlib.h>
};
#include <string>
#include <functional>
#include <unordered_map>
#include <utility>




namespace lua
{
	using raw_state = ::lua_State;

	struct state
	{
		using self_type = state;
		using name_type = std::string;
		using func_type = std::function<int(self_type&)>;
		using library_type = std::unordered_map<name_type, func_type>;
		using number_type = lua_Number;
		using string_type = std::string;
		using cfunc_type = lua_CFunction;


		state(raw_state* s) : rs(s), owned(false) {}
		state() : rs(luaL_newstate()), owned(true) {}
		~state() { if (owned) lua_close(rs); }


		state(const self_type&) = delete;
		self_type& operator=(const self_type&) = delete;
		state(self_type&&) = delete;
		self_type& operator=(self_type&&) = delete;

		operator raw_state*() { return rs; }


		int upvalue_index(int i) { return lua_upvalueindex(i); }

		template <class ...Args>
		void error(Args&& ...args)
		{
			luaL_error(rs, std::forward<Args>(args)...);
		}


		void table(int narr=0, int nrec=0)
		{
			lua_createtable(rs, narr, nrec);
		}

		void field(int index, const string_type& key)
		{
			lua_setfield(rs, index, key.c_str());
		}

		void push(const string_type& str)
		{
			lua_pushlstring(rs, str.c_str(), str.size());
		}

		void push(number_type num)
		{
			lua_pushnumber(rs, num);
		}

		void push(cfunc_type func, int nclosure=0)
		{
			lua_pushcclosure(rs, func, nclosure);
		}

		template <class T>
		void push(T* ptr)
		{
			auto p = reinterpret_cast<void*>(ptr);
			lua_pushlightuserdata(rs, p);
		}

		// the function must not be destroyed after calling this!
		void push(func_type& func)
		{
			push(&func);
			push(func_caller, 1);
		}

		// the library must not be destroyed after calling this!
		void push(library_type& lib)
		{
			table(0, lib.size());
			for (auto& entry: lib) {
				push(entry.second);
				field(-2, entry.first);
			}
		}

		template <class T>
		void get(T*& ptr, int index)
		{
			if (!lua_islightuserdata(rs, index))
				error("pointer expected");
			ptr = reinterpret_cast<T*>(lua_touserdata(rs, index));
		}

		void get(number_type& num, int index)
		{
			num = luaL_checknumber(rs, index);
		}

		void get(string_type& str, int index)
		{
			if (!lua_isstring(rs, index))
				error("string expected");
			size_t len;
			auto cstr = lua_tolstring(rs, index, &len);
			str = std::move(string_type(cstr, cstr+len));
		}

		void get(cfunc_type& func, int index)
		{
			if (!lua_iscfunction(rs, index))
				error("c function expected");
			func = lua_tocfunction(rs, index);
		}

	private:
		raw_state* rs;
		bool owned;

		static int func_caller(raw_state* rs)
		{
			self_type s(rs);
			func_type* func_ptr;
			s.get(func_ptr, s.upvalue_index(1));
			func_type& func = *func_ptr;
			return func(s);
		}
	};

	using library = state::library_type;
	using number  = state::number_type;
	using string  = state::string_type;
	using cfunction = state::cfunc_type;
};

