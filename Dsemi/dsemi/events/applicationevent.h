#pragma once
#include <string>
#include <sstream>

#include "dsemi/events/event.h"
#include "dsemi/core/window.h"

namespace dsemi {
	class WindowResizeEvent : public ievent
	{
	public:
		WindowResizeEvent(unsigned int width, unsigned int height)
			: _width(width), _height(height)
		{
		}

		inline unsigned int width() const { return _width; }
		inline unsigned int height() const { return _height; }

		std::string to_string() const override
		{
			std::stringstream ss;
			ss << "WindowResizeEvent: " << _width << ", " << _height;
			return ss.str();
		}

		EVENT_CLASS_TYPE(EVENT_WINDOW_RESIZE)
		EVENT_CLASS_GROUP(EVENT_GROUP_APPLICATION)
	private:
		unsigned int _width, _height;
	};

	class window_move_event : public ievent
	{
	public:
		window_move_event(float x, float y)
			: _x(x), _y(y)
		{
		}

		inline float x() const { return _x; }
		inline float y() const { return _y; }

		std::string to_string() const override
		{
			std::stringstream ss;
			ss << "WindowMovedEvent: " << _x << ", " << _y;
			return ss.str();
		}

		EVENT_CLASS_TYPE(EVENT_WINDOW_MOVE)
		EVENT_CLASS_GROUP(EVENT_GROUP_APPLICATION)
	private:
		float _x, _y;
	};

	class WindowCloseEvent : public ievent
	{
	public:
		WindowCloseEvent(window* wnd)
			: _wnd(wnd)
		{
		}

		std::string to_string() const override
		{
			// TODO: for the future include the handle to the window that has been closed
			// for multi windowed applications
			return "WINDOWCLOSEEVENT[]";
		}

		inline window* GetPointer() { return _wnd; }

		EVENT_CLASS_TYPE(EVENT_WINDOW_CLOSE)
		EVENT_CLASS_GROUP(EVENT_GROUP_APPLICATION)
	private:
		window* _wnd;
	};

	class window_focus_event : public ievent
	{
	public:
		window_focus_event(window* wnd)
			: _wnd(wnd)
		{
		}

		std::string to_string() const override
		{
			// TODO: for the future include the handle to the window that has been closed
			// for multi windowed applications
			return "WINDOW_FOCUS_EVENT[]";
		}

		EVENT_CLASS_TYPE(EVENT_WINDOW_FOCUS)
		EVENT_CLASS_GROUP(EVENT_GROUP_APPLICATION)
	private:
		window* _wnd;
	};

	class window_focus_lost_event : public ievent
	{
	public:
		window_focus_lost_event(window* wnd)
			: _wnd(wnd)
		{
		}

		std::string to_string() const override
		{
			// TODO: for the future include the handle to the window that has been closed
			// for multi windowed applications
			return "EVENT_WINDOW_FOCUS_LOST[]";
		}

		EVENT_CLASS_TYPE(EVENT_WINDOW_FOCUS_LOST)
		EVENT_CLASS_GROUP(EVENT_GROUP_APPLICATION)
	private:
		window* _wnd;
	};

	class app_tick_event : public ievent
	{
	public:
		app_tick_event()
		{
		}

		std::string to_string() const override
		{
			// MAYBE: add the elapsed time since last call as info?
			return "APP_TICK_EVENT[]";
		}

		EVENT_CLASS_TYPE(EVENT_APP_TICK)
		EVENT_CLASS_GROUP(EVENT_GROUP_APPLICATION)
	};

	class app_update_event : public ievent
	{
	public:
		app_update_event()
		{
		}

		std::string to_string() const override
		{
			// MAYBE: add the elapsed time since last call as info?
			return "APP_UPDATE_EVENT[]";
		}

		EVENT_CLASS_TYPE(EVENT_APP_UPDATE)
		EVENT_CLASS_GROUP(EVENT_GROUP_APPLICATION)
	};

	class app_render_event : public ievent
	{
	public:
		app_render_event()
		{
		}

		std::string to_string() const override
		{
			// MAYBE: add the elapsed time since last call as info?
			return "APP_RENDER_EVENT[]";
		}

		EVENT_CLASS_TYPE(EVENT_APP_RENDER)
		EVENT_CLASS_GROUP(EVENT_GROUP_APPLICATION)
	};
}
