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
			event_variant evar;
			evar.template emplace<EVENT>(std::move(ev));

			lock _(m);
			while (priority_compare<EVENT::priority()>())
				events.pop_back();
			events.emplace_back(std::move(evar));
			cv.notify_one();
		}

		template <class EVENT>
		bool is()
		{
			lock _(m);
			if (!events.size()) return false;
			return events.front().template is<EVENT>();
		}

		template <class EVENT>
		auto get()	// you should be sure there is a event
		{
			event_variant evar;
			{
				lock _(m);
				evar = std::move(events.front());
				events.erase(events.begin());
			}
			return evar.template strict_get<EVENT>();
		}

		template <class EVENT>
		auto wait()
		{
			event_variant evar;
			{
				lock _(m);
				cv.wait(_, [this] { return events.size(); });
				evar = std::move(events.front());
				events.erase(events.begin());
			}
			return evar.template strict_get<EVENT>();
		}

		template <class F>
		decltype(auto) listen(F const& f={})
		{
			event_variant evar;
			{
				lock _(m);
				cv.wait(_, [this] { return events.size(); });
				evar = std::move(events.front());
				events.erase(events.begin());
			}
			return evar.visit(f);
		}

	private:
		using condv = std::condition_variable;
		using mutex = std::mutex;
		using lock  = std::unique_lock<mutex>;

		using event_variant = meta::variant<EVENTS...>;
		using event_list = std::vector<event_variant>;	// FIXME: vector or deque, that is a question

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

