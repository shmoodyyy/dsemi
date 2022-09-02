#ifndef HEADER_DSEMI_MOUSE_EVENT
#define HEADER_DSEMI_MOUSE_EVENT
#include "dsemi/events/event.h"

namespace dsemi {
	class mouse_move_event : public ievent
	{
	public:
		mouse_move_event(float x, float y)
			: _mouse_x(x), _mouse_y(y)
		{
		}

		inline float GetX() const { return _mouse_x; }
		inline float GetY() const { return _mouse_y; }

		std::string to_string() const override
		{
			std::ostringstream ss;
			ss << "EVENT_MOUSE_MOVE [x:" << _mouse_x << " | y:" << _mouse_y << "]";
			return ss.str();
		}

		EVENT_CLASS_TYPE(EVENT_MOUSE_MOVE);
		EVENT_CLASS_GROUP(EVENT_GROUP_MOUSE);
	private:
		float _mouse_x, _mouse_y;
	};

	class mouse_button_event : public ievent
	{
	public:
		inline int GetMouseButton() const { return _mouse_button; }

		EVENT_CLASS_GROUP(EVENT_GROUP_MOUSE | EVENT_GROUP_MOUSE_BUTTON);
	protected:
		mouse_button_event(int button)
			: _mouse_button(button)
		{
		}

		int _mouse_button;
	};

	class mouse_press_event : public mouse_button_event
	{
	public:
		mouse_press_event(int button)
			: mouse_button_event(button)
		{
		}

		std::string to_string() const override
		{
			std::stringstream ss;
			ss << "MOUSE_PRESS_EVENT[" << _mouse_button<< "]";
			return ss.str();
		}

		EVENT_CLASS_TYPE(EVENT_MOUSE_PRESS);
	};

	class mouse_release_event : public mouse_button_event
	{
	public:
		mouse_release_event(int button)
			: mouse_button_event(button)
		{
		}

		std::string to_string() const override
		{
			std::stringstream ss;
			ss << "MOUSE_RELEASE_EVENT[" << _mouse_button << "]";
			return ss.str();
		}

		EVENT_CLASS_TYPE(EVENT_MOUSE_RELEASE);
	};

	class MouseScrolledEvent : public ievent
	{
	public:
		MouseScrolledEvent(short scroll_delta)
			: _scroll_delta(scroll_delta)
		{
		}

		std::string to_string() const override
		{
			std::stringstream ss;
			ss << "MOUSE_SCROLL_EVENT[" << _scroll_delta << "]";
			return ss.str();
		}

		inline short GetDelta() const
		{
			return _scroll_delta;
		}

		EVENT_CLASS_TYPE(EVENT_MOUSE_SCROLL);
		EVENT_CLASS_GROUP(EVENT_GROUP_MOUSE);
	private:
		short _scroll_delta;
	};
}

#endif