#include "dspch.h"

// move this include and ImGui implementation to a seperate graphics system
#define DIRECTINPUT_VERSION 0x0800

#include "Dsemi/Core/window.h"

namespace dsemi {

	window* window::main_wnd = nullptr;

	window::window()
	{
	}

	window* const window::get_wnd() noexcept {
		return window::main_wnd;
	}

	// Constructor
	window::window(unsigned int width, unsigned int height, const std::wstring& title)
	{
		ASSERT(main_wnd == nullptr, "Attempted to create a new window! Multi-Window support not implemented yet.");
		try
		{
			create(width, height, title);
			main_wnd = this;
		}
		catch (dsemi_exception& e)
		{
			display_except(e.what(), e.type());
		}
		catch (std::exception& e)
		{
			display_except(e.what(), "Standard Exception");
		}
		catch (...)
		{
			display_except("No Info.", "Unknown Exception");
		}
	}

	// Destructor
	window::~window()
	{
		close();
	}
}