// move this include and ImGui implementation to a seperate graphics system
#define DIRECTINPUT_VERSION 0x0800
#include "window.h"

namespace
{
    void defaultCallback(dsemi::ievent&) { return; }
}

namespace dsemi 
{
    auto Window::width() -> unsigned int const
    {
        return m_width;
    }

    auto Window::height() -> unsigned int const
    {
        return m_height;
    }

    auto Window::getSwapChain() -> std::shared_ptr<graphics::SwapChain>
    {
        return m_swapChain;
    }

	void Window::set_focus(bool value)
	{
		m_isFocused = value;
	}

	void Window::set_event_callback(eventCallback&& callback)
	{
		m_eventCallback = callback;
	}

}
