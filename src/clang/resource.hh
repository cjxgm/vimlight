#pragma once
#include <utility>
#include <functional>

namespace clang
{
	namespace resource
	{
		// a pod container
		// should not be used for polymorphism
		template <class VALUE>
		struct bin
		{
			using self = bin;
			using value_type = VALUE;

			bin() = default;
			bin(value_type value) : value(value) {}

			value_type const& get() const { return value; }
			operator value_type() const { return get(); }

			void set(value_type const& v) { value = v; }

		private:
			value_type value;
		};

		// an auto-delete pod container like unique_ptr for pointers
		// should not be used for polymorphism
		//
		//invariant:
		// if (owned) assert(deleter);
		template <class VALUE>
		struct unique : bin<VALUE>
		{
			using value_type = VALUE;
			using self = unique;
			using super = bin<VALUE>;
			using deleter_type = std::function<void(value_type)>;

			unique(deleter_type d) : owned(false), deleter(d) {}
			unique(value_type value, deleter_type d)
				: super(value), owned(true), deleter(d) {}
			~unique() { delete_if_owned(); }

			// no copy
			unique(self const&) = delete;
			self& operator=(self const&) = delete;

			unique(self&& other)
				:	super(other.get()),
					owned(other.owned),
					deleter(std::move(other.deleter))
			{
				other.owned = false;
			}

			self& operator=(self&& other)
			{
				delete_if_owned();
				super::set(other.get());
				deleter = std::move(other.deleter);
				owned = other.owned;
				other.owned = false;
				return *this;
			}

			void set(value_type const& v)
			{
				delete_if_owned();
				owned = true;
				super::set(v);
			}

		private:
			bool owned;
			deleter_type deleter;

			void delete_if_owned() { if (owned) deleter(super::get()); }
		};
	}
}

