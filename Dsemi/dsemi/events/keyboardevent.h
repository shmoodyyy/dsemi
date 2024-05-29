#ifndef HEADER_DSEMI_EVENTS_KEYBOARDEVENT
#define HEADER_DSEMI_EVENTS_KEYBOARDEVENT

#include "dsemi/events/event.h"

namespace dsemi {
	class key_event : public ievent
	{
	public:
		key_event(unsigned int keycode)
			:_keycode(keycode)
		{
		}

		inline unsigned int GetKey() const { return _keycode; }

		EVENT_CLASS_GROUP(EVENT_GROUP_INPUT)
	protected:
		unsigned int _keycode;
	};

	class key_down_event : public key_event
	{
	public:
		key_down_event(unsigned int keycode)
			: key_event(keycode)
		{
		}

		std::string to_string() const override
		{
			std::stringstream ss;
			ss << "KEY_DOWN_EVENT[" << _keycode << "]";
			return ss.str();
		}

		EVENT_CLASS_TYPE(EVENT_KEY_DOWN)
	};

	class key_press_event : public key_event
	{
	public:
		key_press_event(unsigned int keycode)
			: key_event(keycode)
		{
		}

		std::string to_string() const override
		{
			std::stringstream ss;
			ss << "KEY_PRESS_EVENT[" << _keycode << "]";
			return ss.str();
		}

		EVENT_CLASS_TYPE(EVENT_KEY_PRESS)
	};

	class key_release_event : public key_event
	{
	public:
		key_release_event(unsigned int keycode)
			: key_event(keycode)
		{
		}

		std::string to_string() const override
		{
			std::stringstream ss;
			ss << "KEY_RELEASE_EVENT[" << _keycode << "]";
			return ss.str();
		}

		EVENT_CLASS_TYPE(EVENT_KEY_RELEASE)
	};
}

#endif