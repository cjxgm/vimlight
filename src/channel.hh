#pragma once
// an inter-thread multiple-writer single-reader communication channel.
// this is used to pass events to another thread.
// this is prioritized, i.e. a higher priority events will erase
// subsequent lower priority ones.
#include <mutex>
#include <condition_variable>
#include <vector>
#include <utility>		// for std::move
#include "meta/variant.hh"

namespace vimlight
{
	template <int PRIORITY>
	struct event
	{
		static constexpr auto priority() { return PRIORITY; }
	};

	template <class ...EVENTS>
	struct channel
	{
		template <class EVENT>
		void post(EVENT ev={})
		{
			lock _(m);
			while (priority_compare<EVENT::priority()>())
				events.pop_back();
			events.emplace_back(std::move(ev));
			cv.notify_one();
		}

		template <class EVENT>
		bool ignore()
		{
			lock _(m);
			if (!events.size()) return false;
			if (!events.back().template is<EVENT>()) return false;
			events.pop_back();
			return true;
		}

		template <class EVENT>
		auto wait()
		{
			event_variant ev;
			{
				lock _(m);
				cv.wait(_, [this] { return events.size(); });
				ev = std::move(events.back());
				events.pop_back();
			}
			if (ev.template is<EVENT>()) return ev;
			throw ev;
		}

		template <class F>
		void listen(F const& f={})
		{
			event_variant ev;
			{
				lock _(m);
				cv.wait(_, [this] { return events.size(); });
				ev = std::move(events.back());
				events.pop_back();
			}
			ev.visit(f);
		}

	private:
		using condv = std::condition_variable;
		using mutex = std::mutex;
		using lock  = std::unique_lock<mutex>;

		using event_variant = meta::variant<EVENTS...>;
		using event_list = std::vector<event_variant>;

		condv cv;
		mutex m;
		event_list events;

		template <int PRIORITY>
		bool priority_compare()
		{
			if (!events.size()) return false;
			return events.back().visit(
					[](auto& ev) {
						return (ev.priority() <= PRIORITY);
					});
		}
	};
}

