#ifndef DSEMI_CORE_WINDOW_H
#define DSEMI_CORE_WINDOW_H

#include <unordered_map>

#include "dsemi/math/vector2.h"
#include "dsemi/Events/Event.h"
#include "dsemi/core/DsemiWindows.h"

namespace dsemi {

	/*
		Abstraction class for native OS windows
	*/
	class window
	{
	public:
		using event_callback_fn = std::function<void(ievent&)>;

	public:
		window();
		window(unsigned int width, unsigned int height, const std::wstring& title);
		~window();

		void create(unsigned int width, unsigned int height, const std::wstring& title);
		void close();

		void dispatch_events();

		inline unsigned int width()  const noexcept { return _data.width; }
		inline unsigned int height() const noexcept { return _data.height; }

		inline bool get_focused() const noexcept { return _focused; }
		inline void set_focused(bool val) noexcept { _focused = val; }

		inline void set_event_callback(const event_callback_fn& callback) { _data.event_callback = callback; };

		// Static members
	public:


		// this is temporary dear god please remove this when you start handling multi-window support
	public:
		static window* const get_wnd() noexcept;
	private:
		static window* main_wnd;

		// Per-object members
	private:
		bool _initialized;
		bool _focused;

		struct window_data
		{
			std::wstring title;
			unsigned int width;
			unsigned int height;

			event_callback_fn event_callback;
		};

		window_data _data;

		/*=== Per platform defines ===*/
		/*----------------------------*/
		/*=== WIN32/WIN64 ===*/
#if ( defined _WIN32 | defined _WIN64 )
	private:
		static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
		static std::unordered_map<HWND, window*> _window_map;

	private:
		// Win32 window objects
		static WNDCLASSEX _wnd_class;
		HWND _hwnd;
		HINSTANCE _hinst;

		// Mapping Window Handles to specific objects for event handling
		//static window* _main_window; // adress for the main window for quit message handling (might not be neccessary actually)


	public:
		inline HWND hwnd() const noexcept {
			return _hwnd;
		}

		/*=== MacOSX ===*/
#elif
		// ahahaha yea sure buddy


		/*=== Linux/UNIX ===*/
#elif
		// ahah


#endif
	};
	
#if (defined _WIN32 | defined _WIN64)

#endif
}

#endif