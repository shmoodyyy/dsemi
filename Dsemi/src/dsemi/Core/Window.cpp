#include "dspch.h"

// move this include and ImGui implementation to a seperate graphics system
#define DIRECTINPUT_VERSION 0x0800

#include "Dsemi/core/window.h"

#include "dsemi/core/application.h"
#include "dsemi/core/input.h"

#include "dsemi/events/applicationevent.h"
#include "dsemi/events/keyboardevent.h"
#include "dsemi/events/mouseevent.h"

#include "dsemi/graphics/device.h"

#include "dsemi/util/logger.hpp"



#include <iostream>

namespace dsemi {

	window::window()
		:_hwnd(NULL)
	{
	}

	// Constructor
	window::window(unsigned int width, unsigned int height, const std::wstring& title)
		:_hwnd(NULL)
	{
		//// default event callback to avoid trying to dispatch events into the void
		//application& app = *application::get();
		//set_event_callback(BIND_EVENT(application::handle_event));
		create(width, height, title);
	} // class window

	// Destructor
	window::~window()
	{
		close();
	}
	void window::set_focus(bool value) noexcept
	{
		_focused = value;
	}
	void window::set_event_callback(event_callback_fn&& callback) noexcept
	{
		_data.event_callback = callback;
	}

	void window::create(unsigned int width, unsigned int height, const std::wstring& title)
	{
		if (_hwnd)
			close();

		_data.title = title;
		_data.width = width;
		_data.height = height;

		RECT wr;
		wr.left = 100;
		wr.right = _data.width + wr.left;
		wr.top = 100;
		wr.bottom = _data.height + wr.top;
		// Adjust the size of entire window so that the client region rect is equal to the width and height
		AdjustWindowRect(&wr, WS_CAPTION | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);

		_hwnd = CreateWindowEx(
			0,
			win32_window_class::get_name(),
			_data.title.c_str(),
			WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU | WS_THICKFRAME,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			wr.right - wr.left,
			wr.bottom - wr.top,
			nullptr,
			nullptr,
			win32_window_class::get_hinstance(),
			this
		);

		if (!_hwnd)
		{
			unsigned int nResult = GetLastError();
			throw CUSTOM_EXCEPT("Failed Creating HWnd");
		}
		ShowWindow(_hwnd, SW_SHOWDEFAULT);

		RAWINPUTDEVICE rid;
		rid.usUsagePage = 0x01;
		rid.usUsage = 0x02;
		rid.dwFlags = 0u;
		rid.hwndTarget = nullptr;
		if (!RegisterRawInputDevices(&rid, 1u, sizeof(rid)))
		{
			unsigned int nResult = GetLastError();
			throw CUSTOM_EXCEPT("Failed Registering Raw Input Device");
		}

		return;
	}

	void window::close()
	{
		DestroyWindow(_hwnd);
		return;
	}

	LRESULT CALLBACK window::_setup_wnd_proc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	LRESULT window::_wnd_proc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		// i hate this so much
		// TODO: rewrite all this bullshit please
		window* wnd = nullptr;

		if (!wnd)
		{
			//__debugbreak(); // summin must have gone horribly wrong // maybe not IDK AAAAAAAA AAA A A- one 12 gauge please thnka!
			return DefWindowProc(hwnd, msg, wParam, lParam);
		}

		switch (msg)
		{
		case WM_ACTIVATE:
		{
			if (wParam == WA_ACTIVE || wParam == WA_CLICKACTIVE)
			{
				set_focus(true);
				window_focus_event e(this);
				_data.event_callback(e);
			}
			else if (wParam == WA_INACTIVE)
			{
				set_focus(false);
				window_focus_lost_event e(this);
				_data.event_callback(e);
			}
		}
		break;
		case WM_KEYDOWN:
		{
			key_press_event e_repeat((unsigned int)wParam);
			key_down_event e((unsigned int)wParam);
			if ((lParam & (1 << 30)) == 0)
				_data.event_callback(e); // first message for key down
			_data.event_callback(e_repeat);
		}
		break;
		case WM_KEYUP:
		{
			key_release_event e((unsigned int)wParam);
			Input::Instance()->OnKeyUp(wParam);
			wnd->_data.event_callback(e);
		}
		break;
		// verbose boilerplate for supporting different mouse buttons
		case WM_LBUTTONDOWN:
		{
			mouse_press_event e(1);
			wnd->_data.event_callback(e);
		}
		break;
		case WM_LBUTTONUP:
		{
			mouse_release_event e(1);
			wnd->_data.event_callback(e);
		}
		break;
		case WM_RBUTTONDOWN:
		{
			mouse_press_event e(2);
			wnd->_data.event_callback(e);
		}
		break;
		case WM_RBUTTONUP:
		{
			mouse_release_event e(2);
			wnd->_data.event_callback(e);
		}
		break;
		case WM_MBUTTONDOWN:
		{
			mouse_press_event e(3);
			wnd->_data.event_callback(e);
		}
		break;
		case WM_MBUTTONUP:
		{
			mouse_release_event e(3);
			wnd->_data.event_callback(e);
		}
		break;
		case WM_XBUTTONDOWN:
		{
			short button_code = HIWORD(wParam);
			mouse_press_event e(3 + button_code);
			wnd->_data.event_callback(e);
		}
		break;
		case WM_XBUTTONUP:
		{
			short button_code = HIWORD(wParam);
			mouse_release_event e(3 + button_code);
			wnd->_data.event_callback(e);
		}
		break;
		case WM_MOUSEWHEEL:
		{
			short zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
			mouse_scroll_event e(zDelta);
			wnd->_data.event_callback(e);
		}
		break;
		case WM_MOUSEMOVE:
		{
			mouse_move_event e(LOWORD(lParam), HIWORD(lParam));
			vector2i newPos = vector2i((int)e.GetX(), (int)e.GetY());
			if (!Input::IsMouseRawInput())
			{
				Input::s_mouseDelta += Input::s_mousePos - newPos;
			}
			Input::s_mousePos = newPos;
			wnd->_data.event_callback(e);
		}
		break;
		case WM_SIZE:
		{
			// prevent memory access exceptions, as WM_SIZE is called as soon as the window is created
			if (!wnd->_data.event_callback)
			{
				break;
			}
			wnd->_data.width = LOWORD(lParam);
			wnd->_data.height = HIWORD(lParam);

			// resize render context
			// renderContext.UpdateBuffers(vector2f((float)m_Data.Width, (float)m_Data.Height));

			// send event
			window_resize_event e(wnd->_data.width, wnd->_data.height);
			wnd->_data.event_callback(e);
		}
		break;
		case WM_CLOSE:
		{
			window_close_event e(wnd);
			wnd->_data.event_callback(e);
			wnd->close();
		}
		break;
		case WM_QUIT:
		{
			window_close_event e(wnd);
			wnd->_data.event_callback(e);
			PostQuitMessage(0);
		}
		break;
		case WM_INPUT:
		{
			if (!Input::IsMouseRawInput())
				break;

			UINT dwSize = 0u;
			GetRawInputData((HRAWINPUT)lParam, RID_INPUT, nullptr, &dwSize, sizeof(RAWINPUTHEADER));
			LPBYTE lpb = new BYTE[dwSize];
			if (!lpb)
				return 0;
			if (GetRawInputData((HRAWINPUT)lParam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER)) != dwSize)
				break;

			RAWINPUT* pRaw = (RAWINPUT*)lpb;
			if (pRaw->header.dwType == RIM_TYPEMOUSE)
			{
				Input::s_mouseDelta += vector2i((int)pRaw->data.mouse.lLastX, (int)pRaw->data.mouse.lLastY);
			}
		}
		break;
		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
		}
		return 0;
	}

	// win32 window class
	window::win32_window_class window::win32_window_class::_instance;

	window::win32_window_class::win32_window_class()
	{
		_hinstance = GetModuleHandle(nullptr);

		WNDCLASSEX wnd_class = {};
		wnd_class.cbSize = sizeof(WNDCLASSEX);
		wnd_class.style = CS_HREDRAW | CS_VREDRAW;
		wnd_class.lpfnWndProc = window::_setup_wnd_proc;
		wnd_class.cbClsExtra = 0;
		wnd_class.cbWndExtra = 0;
		wnd_class.hInstance = _hinstance;
		wnd_class.hIcon = nullptr;
		wnd_class.hCursor = LoadCursor(NULL, IDC_ARROW);
		wnd_class.hbrBackground = nullptr;
		wnd_class.lpszMenuName = nullptr;
		wnd_class.lpszClassName = _win32_window_class_name;
		wnd_class.hIconSm = nullptr;

		if (!RegisterClassEx(&wnd_class))
		{
			unsigned int nResult = GetLastError();
			throw CUSTOM_EXCEPT("Failed Registering window Class");
		}
	}

	window::win32_window_class::~win32_window_class()
	{
		UnregisterClass(get_name(), get_hinstance());
	}

} // namespace dsemi