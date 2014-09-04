#pragma once

namespace clang
{
	namespace internal
	{
		template <class Value>
		struct bin
		{
			using self_type = bin;
			using value_type = Value;

			bin() = default;
			bin(value_type value) : value(value) {}

			virtual ~bin() = default;

			const value_type& get() const { return value; }
			operator value_type() const { return get(); }

		protected:
			void set(const value_type& v) { value = v; }

		private:
			value_type value;
		};

		template <class Value>
		struct guard : public bin<Value>
		{
			using self_type = guard;
			using super_type = bin<Value>;
			using value_type = typename super_type::value_type;

			guard() : owned(false) {}
			guard(value_type value) : super_type(value), owned(true) {}

			guard(const self_type&) = delete;
			self_type& operator=(const self_type&) = delete;

			guard(self_type&& other) : super_type(other.get()), owned(true)
			{
				other.owned = false;
			}
			self_type& operator=(self_type&& other)
			{
				set(other.get());
				owned = true;
				other.owned = false;
				return *this;
			}

		protected:
			bool owned;
		};
	}
}

