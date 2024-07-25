// move this include and ImGui implementation to a seperate graphics system
#define DIRECTINPUT_VERSION 0x0800
#include "window.h"

namespace dsemi 
{
    auto Window::getWidth() -> unsigned const
    {
        return m_width;
    }

    auto Window::getHeight() -> unsigned const
    {
        return m_height;
    }

    auto Window::getSwapChain() -> std::shared_ptr<graphics::SwapChain>
    {
        return m_swapChain;
    }

    auto Window::getRenderTarget() -> std::shared_ptr<graphics::RenderTarget>
    {
        return m_swapChain;
    }

	void Window::setFocus(bool value)
	{
		m_isFocused = value;
	}

	void Window::setEventCallback(eventCallback&& callback)
	{
		m_eventCallback = callback;
	}

}
