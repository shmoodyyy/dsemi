#define DIRECTINPUT_VERSION 0x0800
#include "dsemi/core/window.h"
#include "dsemi/core/input.h"
#include "dsemi/events/mouseevent.h"
#include "dsemi/events/keyboardevent.h"
#include "dsemi/events/applicationevent.h"
#include "dsemi/math/vector2.h"

namespace
{
    void defaultCallback(dsemi::ievent&) { return; }
}

// i dont know how im going to make this cross-platform compilable in a neat way
dsemi::Window::Window(unsigned int width, unsigned int height, std::string_view title)
    : m_width(width)
    , m_height(height)
    , m_title(title)
    , m_eventCallback(defaultCallback)
      , m_hwnd(0)
{
    RECT wr;
    wr.left = 100;
    wr.right = m_width + wr.left;
    wr.top = 100;
    wr.bottom = m_height + wr.top;
    AdjustWindowRect(&wr, WS_CAPTION | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);
    m_hwnd = CreateWindowEx(
        0,
        win32_window_class::get_name(),
        m_title.c_str(),
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
    if (!m_hwnd)
        throw CUSTOM_EXCEPT("Failed Creating HWnd");
    ShowWindow(m_hwnd, SW_SHOWDEFAULT);

    RAWINPUTDEVICE rid;
    rid.usUsagePage = 0x01;
    rid.usUsage = 0x02;
    rid.dwFlags = 0u;
    rid.hwndTarget = nullptr;
    if (!RegisterRawInputDevices(&rid, 1u, sizeof(rid)))
        throw CUSTOM_EXCEPT("Failed Registering Raw Input Device");
    m_swapChain = std::make_shared<graphics::SwapChain>(*this);
}

dsemi::Window::~Window()
{
    DestroyWindow(m_hwnd);
}

auto dsemi::Window::getHwnd() -> HWND
{
    return m_hwnd;
}

void dsemi::Window::resize(unsigned width, unsigned height)
{
    m_width = width;
    m_height = height;
    if (m_swapChain)
        m_swapChain->resize(width, height);
    WindowResizeEvent e(m_width, m_height);
    m_eventCallback(e);
}

LRESULT CALLBACK dsemi::Window::_setup_wnd_proc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (msg == WM_NCCREATE) {
        const CREATESTRUCT* wnd_create = reinterpret_cast<CREATESTRUCT*>(lParam);
        if (wnd_create->lpCreateParams) {
            Window* wnd_ptr = reinterpret_cast<Window*>(wnd_create->lpCreateParams);
            SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(wnd_ptr));
            SetWindowLongPtr(hwnd, GWLP_WNDPROC,  reinterpret_cast<LONG_PTR>(dsemi::Window::_redirect_wnd_proc));
            return dsemi::Window::_redirect_wnd_proc(hwnd, msg, wParam, lParam);
        }
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

LRESULT CALLBACK dsemi::Window::_redirect_wnd_proc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    Window* wnd = reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    return wnd->_wnd_proc(hwnd, msg, wParam, lParam);
}

LRESULT dsemi::Window::_wnd_proc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    // we are at last in an instance-level function call
    switch (msg) {
    case WM_ERASEBKGND: {
        break;
    }
    case WM_ACTIVATE: {
        if (wParam == WA_ACTIVE || wParam == WA_CLICKACTIVE) {
            setFocus(true);
            window_focus_event e(this);
            m_eventCallback(e);
        }
        else if (wParam == WA_INACTIVE) {
            setFocus(false);
            window_focus_lost_event e(this);
            m_eventCallback(e);
        }
        break;
    }
    case WM_KEYDOWN: {
        key_press_event e_repeat((unsigned int)wParam);
        KeyDownEvent e((unsigned int)wParam);
        if ((lParam & (1 << 30)) == 0)
            m_eventCallback(e); // first message for key down
        m_eventCallback(e_repeat);
        break;
    }
    case WM_KEYUP: {
        key_release_event e((unsigned int)wParam);
        Input::Instance()->OnKeyUp(wParam);
        m_eventCallback(e);
        break;
    }
    case WM_LBUTTONDOWN: {
        mouse_press_event e(1);
        m_eventCallback(e);
        break;
    }
    case WM_LBUTTONUP: {
        mouse_release_event e(1);
        m_eventCallback(e);
        break;
    }
    case WM_RBUTTONDOWN: {
        mouse_press_event e(2);
        m_eventCallback(e);
        break;
    }
    case WM_RBUTTONUP: {
        mouse_release_event e(2);
        m_eventCallback(e);
        break;
    }
    case WM_MBUTTONDOWN: {
        mouse_press_event e(3);
        m_eventCallback(e);
        break;
    }
    case WM_MBUTTONUP: {
        mouse_release_event e(3);
        m_eventCallback(e);
        break;
    }
    case WM_XBUTTONDOWN: {
        short button_code = HIWORD(wParam);
        mouse_press_event e(3 + button_code);
        m_eventCallback(e);
        break;
    }
    case WM_XBUTTONUP: {
        short button_code = HIWORD(wParam);
        mouse_release_event e(3 + button_code);
        m_eventCallback(e);
        break;
    }
    case WM_MOUSEWHEEL: {
        short zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
        mouse_scroll_event e(zDelta);
        m_eventCallback(e);
        break;
    }
    case WM_MOUSEMOVE: {
        mouse_move_event e(LOWORD(lParam), HIWORD(lParam));
        vector2i newPos = vector2i((int)e.GetX(), (int)e.GetY());
        if (!Input::IsMouseRawInput()) {
            Input::s_mouseDelta += Input::s_mousePos - newPos;
        }
        Input::s_mousePos = newPos;
        m_eventCallback(e);
        break;
    }
    case WM_SIZE: {
        // resize render context
        // renderContext.UpdateBuffers(vector2f((float)m_Data.Width, (float)m_Data.Height));
        resize(LOWORD(lParam), HIWORD(lParam));
        break;
    }
    case WM_SIZING: {
        RECT cur_size = *(RECT*)lParam;
        resize(cur_size.right - cur_size.left, cur_size.bottom - cur_size.top);
        // resize render context
        // renderContext.UpdateBuffers(vector2f((float)m_Data.Width, (float)m_Data.Height));
        break;
    }
    case WM_CLOSE: {
        WindowCloseEvent e(this);
        m_eventCallback(e);
        break;
    }
    case WM_QUIT: {
        WindowCloseEvent e(this);
        m_eventCallback(e);
        PostQuitMessage(0);
        break;
    }
    case WM_INPUT: {
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
            Input::s_mouseDelta += vector2i((int)pRaw->data.mouse.lLastX, (int)pRaw->data.mouse.lLastY);
        break;
    }
    default: return DefWindowProc(hwnd, msg, wParam, lParam);
    }
}

// win32 window class
dsemi::Window::win32_window_class dsemi::Window::win32_window_class::_instance;

dsemi::Window::win32_window_class::win32_window_class()
{
    _hinstance = GetModuleHandle(nullptr);

    WNDCLASSEX wnd_class    = {};
    wnd_class.cbSize        = sizeof(WNDCLASSEX);
    wnd_class.style         = CS_HREDRAW | CS_VREDRAW;
    wnd_class.lpfnWndProc   = dsemi::Window::_setup_wnd_proc;
    wnd_class.cbClsExtra    = 0;
    wnd_class.cbWndExtra    = 0;
    wnd_class.hInstance     = _hinstance;
    wnd_class.hIcon         = nullptr;
    wnd_class.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wnd_class.hbrBackground = nullptr;
    wnd_class.lpszMenuName  = nullptr;
    wnd_class.lpszClassName = win32ClassName;
    wnd_class.hIconSm       = nullptr;

    if (!RegisterClassEx(&wnd_class))
    {
        unsigned int nResult = GetLastError();
        throw CUSTOM_EXCEPT("Failed Registering window Class");
    }
}

dsemi::Window::win32_window_class::~win32_window_class()
{
    UnregisterClass(get_name(), get_hinstance());
}
