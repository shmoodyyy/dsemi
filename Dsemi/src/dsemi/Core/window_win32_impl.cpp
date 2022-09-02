#include "dspch.h"

#if (defined _WIN32 | defined _WIN64)

#define DIRECTINPUT_VERSION 0x0800 // what the fuck is this for?

#include "dsemi/core/input.h"
#include "dsemi/events/applicationevent.h"
#include "dsemi/events/keyboardevent.h"
#include "dsemi/events/mouseevent.h"

#include "dsemi/core/window.h"
#ifdef DEBUG

#include "dsemi/util/logger.hpp"
#include <iostream>

#endif

namespace dsemi {

	WNDCLASSEX window::_wnd_class;
	std::unordered_map<HWND, window*> window::_window_map;

	LRESULT CALLBACK window::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		// i hate this so much
		// TODO: rewrite all this bullshit please
		window* wnd;
		auto it = _window_map.find(hwnd);
		if (it != _window_map.end())
			wnd = it->second;
		else
			wnd = nullptr;

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
				wnd->set_focused(true);
			else if (wParam == WA_INACTIVE)
				wnd->set_focused(false);
		}
		break;
		case WM_KEYDOWN:
		{
			key_press_event e_repeat((unsigned int)wParam);
			key_down_event e((unsigned int)wParam);
			if ((lParam & (1 << 30)) == 0)
				wnd->_data.event_callback(e); // first message for key down
			wnd->_data.event_callback(e_repeat);
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
			MouseScrolledEvent e(zDelta);
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

	void window::create(unsigned int width, unsigned int height, const std::wstring& title)
	{
		if (_initialized)
			close();

		_data.title  = title;
		_data.width  = width;
		_data.height = height;

		RECT wr;
		wr.left = 100;
		wr.right = _data.width + wr.left;
		wr.top = 100;
		wr.bottom = _data.height + wr.top;
		// Adjust the size of entire window so that the client region rect is equal to the width and height
		AdjustWindowRect(&wr, WS_CAPTION | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);

		_hinst = GetModuleHandle(nullptr);

		_wnd_class = {};
		_wnd_class.cbSize = sizeof(WNDCLASSEX);
		_wnd_class.style = CS_HREDRAW | CS_VREDRAW;
		_wnd_class.lpfnWndProc = WndProc;
		_wnd_class.cbClsExtra = 0;
		_wnd_class.cbWndExtra = 0;
		_wnd_class.hInstance = _hinst;
		_wnd_class.hIcon = nullptr;
		_wnd_class.hCursor = LoadCursor(NULL, IDC_ARROW);
		_wnd_class.hbrBackground = nullptr;
		_wnd_class.lpszMenuName = nullptr;
		_wnd_class.lpszClassName = _data.title.c_str();
		_wnd_class.hIconSm = nullptr;

		if (!RegisterClassEx(&_wnd_class))
		{
			unsigned int nResult = GetLastError();
			throw CUSTOM_EXCEPT("Failed Registering window Class");
		}

		_hwnd = CreateWindowEx(
			0,
			_wnd_class.lpszClassName,
			_data.title.c_str(),
			WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU | WS_THICKFRAME,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			wr.right - wr.left,
			wr.bottom - wr.top,
			nullptr,
			nullptr,
			_hinst,
			this
		);

		if (!_hwnd)
		{
			unsigned int nResult = GetLastError();
			throw CUSTOM_EXCEPT("Failed Creating HWnd");
		}
		ShowWindow(_hwnd, SW_SHOWDEFAULT);
		window::_window_map[_hwnd] = this;

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

		// Set up render context
		//renderContext.Init(m_HandleWnd, vector2f((float)m_Data.Width, (float)m_Data.Height));

		window::main_wnd = this;

		return;
	}

	void window::close()
	{
		DestroyWindow(_hwnd);
		return;
	}

	void window::dispatch_events()
	{
		MSG msg;
		BOOL gResult;
		while ((gResult = PeekMessage(&msg, _hwnd, 0, 0, PM_REMOVE)) > 0)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		return;
	}
}

#endif