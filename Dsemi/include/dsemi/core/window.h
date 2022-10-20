#ifndef DSEMI_CORE_WINDOW_H
#define DSEMI_CORE_WINDOW_H

#include <unordered_map>

#include "dsemi/graphics/api_include.h"
#include "dsemi/math/vector2.h"
#include "dsemi/Events/Event.h"
#include "dsemi/core/DsemiWindows.h"

namespace dsemi 
{
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

		HWND get_hwnd() const noexcept { return _hwnd; }

		inline unsigned int width()  const noexcept { return _data.width; }
		inline unsigned int height() const noexcept { return _data.height; }
		bool         has_focus() const noexcept;
		void         set_focus(bool value) noexcept;
		void         set_event_callback(event_callback_fn&& callback) noexcept;

		bool is_fullscreen() const noexcept;
		void set_fullscreen(unsigned int monitor = 0u, bool val = false);

	private:
		void _create_swap_chain();

		void _on_fullscreen();
		void _on_windowed();
		void _on_resize(unsigned int width, unsigned height);
		void _on_focus();
		void _on_focus_lost();

	private:
		bool _focused;
		HWND _hwnd;
		ComPtr<IDXGISwapChain>         _dx_swap_chain;

		struct window_data
		{
			std::wstring title;
			unsigned int width;
			unsigned int height;
			event_callback_fn event_callback;
		} _data; // struct window_data

	private:
		static LRESULT CALLBACK _setup_wnd_proc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
		static LRESULT CALLBACK _redirect_wnd_proc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
		LRESULT _wnd_proc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	private:
		class win32_window_class
		{
		public:
			//inline static WNDCLASSEX get_wnd_class() noexcept { return _instance._hinstance; };
			inline static const wchar_t* get_name()      noexcept { return _win32_window_class_name; }
			inline static HINSTANCE      get_hinstance() noexcept { return _instance._hinstance; };

		private:
			win32_window_class();
			~win32_window_class();
			win32_window_class(const win32_window_class&) = delete;
			win32_window_class& operator=(const win32_window_class&) = delete;

			static constexpr const wchar_t* _win32_window_class_name = L"Dsemi Win32 Window Class Singleton";
			static win32_window_class       _instance;
			HINSTANCE                       _hinstance;
		}; // class win32_window_class

	}; // class window

} // namespace dsemi

#endif