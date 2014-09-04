#pragma once
// an inter-thread multiple-writer single-reader communication channel.
// this is used to pass events to another thread.
#include <functional>
#include <type_traits>
#include <utility>
#include <mutex>
#include <condition_variable>
#include <list>
#include <memory>

namespace vimlight
{
	struct channel
	{
		template <class Event>
		using listener_t = std::function<void(Event)>;
		using listener_no_arg_t = std::function<void()>;

		struct event
		{
			event() = default;
			event(event const&) = default;
			event(event&&) = default;
			event& operator=(event const&) = default;
			event& operator=(event&&) = default;
			virtual ~event() = default;
		};

		using event_ptr  = std::shared_ptr<event>;
		using event_list = std::list<event_ptr>;
		using listener_wrapper = std::function<bool(event*)>;
		using listener_wrapper_list = std::list<listener_wrapper>;
		using listener_remover = std::function<void()>;

		using condv = std::condition_variable;
		using mutex = std::mutex;
		using lock  = std::unique_lock<mutex>;


		template <class Event>
		listener_remover listen(listener_t<Event> lsnr)
		{
			static_assert(std::is_base_of<event, Event>(),
					"Event must be a sub-class of channel::event");

			lock _(m);

			auto it = listeners.insert(listeners.end(),
					[lsnr](event* ev) {
						auto p = dynamic_cast<Event*>(ev);
						if (!p) return false;
						lsnr(std::move(*p));
						return true;
					});

			return [this, it] {
				listeners.erase(it);
			};
		}

		template <class Event>
		listener_remover listen(listener_no_arg_t lsnr)
		{
			static_assert(std::is_base_of<event, Event>(),
					"Event must be a sub-class of channel::event");

			return listen<Event>([lsnr](Event) { lsnr(); });
		}

		template <class Event>
		void post(Event ev)
		{
			static_assert(std::is_base_of<event, Event>(),
					"Event must be a sub-class of channel::event");

			{
				lock _(m);
				events.push_back(std::make_shared<Event>(std::move(ev)));
			}
			cv.notify_one();
		}

		void wait()
		{
			event_ptr ev;
			{
				lock lck(m);
				cv.wait(lck, [this] { return !events.empty(); });
				ev = events.front();
				events.pop_front();
			}
			for (auto& lsnr: listeners)
				if (lsnr(ev.get()))
					return;
			throw *ev.get();
		}

		void poll()
		{
			event_ptr ev;
			{
				lock lck(m);
				if (events.empty()) return;
				ev = events.front();
				events.pop_front();
			}
			for (auto& lsnr: listeners)
				if (lsnr(ev.get()))
					return;
			throw *ev.get();
		}

	private:
		condv cv;
		mutex m;
		listener_wrapper_list listeners;
		event_list events;
	};
}

