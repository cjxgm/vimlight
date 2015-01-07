#pragma once
#include <utility>
#include <functional>

namespace clang
{
	namespace internal
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

		// an auto-delete pod container
		// should not be used for polymorphism
		//
		// if (owned) assert(deleter);
		template <class VALUE>
		struct guard : bin<VALUE>
		{
			using value_type = VALUE;
			using self = guard;
			using super = bin<value_type>;
			using deleter_type = std::function<void(value_type)>;

			guard(deleter_type d) : owned(false), deleter(d) {}
			guard(value_type value, deleter_type d)
				: super(value), owned(true), deleter(d) {}
			~guard() { delete_if_owned(); }

			// no copy
			guard(self const&) = delete;
			self& operator=(self const&) = delete;

			guard(self&& other)
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

