#ifndef HEADER_DSEMI_EVENTS_EVENT
#define HEADER_DSEMI_EVENTS_EVENT
#include <functional>
#include <string>
#include <sstream>

#define BIT_FLAG(x) (1 << x)
#define BIND_EVENT(x) std::bind(&x, this, std::placeholders::_1)

namespace dsemi {
	enum class event_type
	{
		None = 0,
		// window events
		EVENT_WINDOW_CLOSE, 
		EVENT_WINDOW_RESIZE, 
		EVENT_WINDOW_FOCUS, 
		EVENT_WINDOW_FOCUS_LOST, 
		EVENT_WINDOW_MOVE,
		// application events
		EVENT_APP_TICK, 
		EVENT_APP_UPDATE, 
		EVENT_APP_RENDER,
		// keyboard events
		EVENT_KEY_PRESS, 
		EVENT_KEY_RELEASE, 
		EVENT_KEY_DOWN,
		// mouse events
		EVENT_MOUSE_PRESS, 
		EVENT_MOUSE_RELEASE, 
		EVENT_MOUSE_MOVE, 
		EVENT_MOUSE_SCROLL,
	};

	enum event_group
	{
		NONE                       = 0,
		EVENT_GROUP_APPLICATION    = BIT_FLAG(0),
		EVENT_GROUP_INPUT          = BIT_FLAG(1),
		EVENT_GROUP_KEYBOARD       = BIT_FLAG(2),
		EVENT_GROUP_MOUSE          = BIT_FLAG(3),
		EVENT_GROUP_MOUSE_BUTTON   = BIT_FLAG(4),
	};

#define EVENT_CLASS_TYPE(type) static event_type get_static_type() { return event_type::##type; } \
                               virtual event_type get_event_type() const override { return get_static_type(); }
#define EVENT_CLASS_GROUP(group) virtual int get_group_flags() const override { return group; }

	class ievent
	{
		friend class event_dispatcher;
	public:
		virtual event_type get_event_type() const = 0;
		virtual int get_group_flags() const = 0;
		virtual std::string to_string() const = 0;
		
		inline bool in_group(event_group group)
		{
			return get_group_flags() & group;
		}
		bool handled = false;
	};

	class event_dispatcher
	{
		template<typename T>
		using event_function = std::function<bool(T&)>;
	public:
		event_dispatcher(ievent& event)
			: _event(event)
		{
		}

		template<typename T>
		bool dispatch(event_function<T> fn)
		{
			if (_event.get_event_type() == T::get_static_type())
			{
				_event.handled = fn(*(T*)&_event);
				return true;
			}
			return false;
		}

	private:
		ievent& _event;
	};
}

#endif