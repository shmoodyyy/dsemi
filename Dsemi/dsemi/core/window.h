#ifndef DSEMI_CORE_WINDOW_H
#define DSEMI_CORE_WINDOW_H
#include "dsemi/platform/platforminclude.h"
#include "dsemi/Events/Event.h"
#include "dsemi/graphics/swapchain.h"

namespace dsemi 
{
	class Window
	{
	public:
		using eventCallback = std::function<void(ievent&)>;

		Window(unsigned int width, unsigned int height, std::string_view title);
		~Window();

		auto width() -> unsigned const;
		auto height() -> unsigned const;
		auto has_focus() -> bool const;
		void set_focus(bool value);
		void set_event_callback(eventCallback&& callback);

		bool is_fullscreen() const;
		void set_fullscreen(unsigned int monitor = 0u, bool val = false);

        auto getSwapChain() -> std::shared_ptr<graphics::SwapChain>;

	private:
		void onFullscreen();
		void onWindowed();
		void onResize(unsigned int width, unsigned height);
		void onFocus();
		void onFocusLost();

		bool m_isFocused;
        unsigned m_width;
        unsigned m_height;
        std::string m_title;
        std::shared_ptr<graphics::SwapChain> m_swapChain;
		eventCallback m_eventCallback;

#if defined (_WIN32)
    public:
        auto getHwnd() -> HWND;

    private:
		static LRESULT CALLBACK _setup_wnd_proc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
		static LRESULT CALLBACK _redirect_wnd_proc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
		LRESULT _wnd_proc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

        HWND m_hwnd;

		class win32_window_class
		{
		public:
			//inline static WNDCLASSEX get_wnd_class() { return _instance._hinstance; };
			static auto get_name() -> const char* { return win32ClassName; }
			static auto get_hinstance() -> HINSTANCE { return _instance._hinstance; };

		private:
			win32_window_class();
			~win32_window_class();
			win32_window_class(const win32_window_class&) = delete;
			win32_window_class& operator=(const win32_window_class&) = delete;

			static constexpr const char* win32ClassName = "win32Class";
			static win32_window_class _instance;
			HINSTANCE _hinstance;
		};
#endif
	};
}

#endif
