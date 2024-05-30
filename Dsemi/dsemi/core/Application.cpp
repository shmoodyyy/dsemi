#include "dsemi/core/dsemiwindows.h" 
#include "application.h"
#include "dsemi/core/exceptions/dsemiexception.h"
#include "dsemi/core/input.h"
#include "dsemi/util/timer.h"
#include "dsemi/util/assert.h"
// TODO: check below includes on platform dependence
#include "fcntl.h"
#include "io.h"
#include <cmath>
#include <string_view>
using namespace std::literals;

namespace dsemi {
	application* application::_instance = nullptr;

	application::application()
		:_delta_time(0)
		,_tick_rate_actual(0)
		,_tick_rate_desired(0)
		,_time_per_tick(0)
	{
		ASSERT(!_instance, "APP: Application Instance already initialized.");
		if (_instance)
		{
			CUSTOM_EXCEPT("Application Instance already exists! Use Application::Get()");
		}
		_instance = this;
	}

	application::~application()
	{
	}

	void application::init()
	{
#if 1
		AllocConsole();
		freopen("CONOUT$", "w", stdout);
		// WIN32 Console style
		CONSOLE_FONT_INFOEX cfi;
		cfi.cbSize = sizeof(cfi);
		cfi.nFont = 0;
		cfi.dwFontSize.X = 0;
		cfi.dwFontSize.Y = 14;
		cfi.FontFamily = FF_DONTCARE;
		cfi.FontWeight = FW_NORMAL;
		//_setmode(_fileno(stdout), _O_U16TEXT);
		std::wcscpy(cfi.FaceName, L"Consolas");
		// TODO: below might be windows only
		SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
#endif

#ifdef _DEBUG
		logger::set_level(logger::level::LOG_LEVEL_DEBUG);
#else
		logger::set_level(logger::level::LOG_LEVEL_INFO);
#endif
		logger::start_logger();

		LOG_DEBUG(L"dsemi::appliation initializing...");

		// Initialize DirectX and Direct3D
		//Direct3D::InitD3D();

		// initialize graphics framework
		graphics::device::initialize();

		set_tick_rate(60u);

		//m_pMainCamera = std::make_shared<Camera3D>();

		//GFXResourceManager::InitSingleton();

		//m_pMainWnd->renderContext.SetCamera(m_pMainCamera.get());
		
		// run additional intialization routine
		_on_init();
		LOG_DEBUG(L"dsemi::appliation done initializing.");
	}

	// looping section of the application handling update, event handling and render calls
	void application::run()
	{
		try 
		{
			MSG msg = {};
			while (_running)
			{
				if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
				{
					// todo: some sort of event manager/handler separation?
					TranslateMessage(&msg);
					DispatchMessage(&msg);
					// TODO: replace with application-level message handling
					//_main_wnd->dispatch_events();
					do_events();
				}

				// Update
				if (do_tick())
				{
					do_render();
					/*m_pMainWnd->renderContext.NewFrame();
					m_pMainWnd->renderContext.Present();*/
				}
			}
			shutdown();
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

	void application::shutdown()
	{
		logger::stop_logger();

		cleanup();
	}

	void application::cleanup()
	{
		// run additional cleanup routine
		_on_cleanup();
	}

	void application::do_update(const float dt)
	{
		on_update(dt);
		active_scene->handle_update(dt);

		Input::ResetMouseDelta();
	}

	bool application::do_tick()
	{
		_delta_time = _update_timer.Peek();

		if (_delta_time >= _time_per_tick)
		{	
			unsigned short numTicks = (unsigned short)std::floor(_delta_time / _time_per_tick); // Get the number of ticks that should have been processed since last frame
			//m_dt -= m_timePerTick * numTicks;
			_update_timer.Mark();

			do_update(_delta_time);
			//active_scene->handle_render(_delta_time);
			return true;
		}
		else
		{
			return false;
		}
	}

	void application::set_tick_rate(unsigned int tickRate) noexcept
	{
		_tick_rate_desired = tickRate;
		if (tickRate != 0)
			_time_per_tick = 1.0f / (float)tickRate;
	}

	void application::do_events()
	{
	}

	void application::do_render()
	{
	}

	// Event Handler
	void application::handle_event(ievent& e)
	{
		event_dispatcher dispatcher(e);
		dispatcher.dispatch<window_close_event>(BIND_EVENT(application::_on_window_close));
		dispatcher.dispatch<window_resize_event>(BIND_EVENT(application::_on_window_resize));
		dispatcher.dispatch<key_down_event>(BIND_EVENT(application::_on_key_down));

		if (!e.handled) {
			on_event(e);
		}

		active_scene->handle_event(e);
	}

	// Event Functions
	bool application::_on_window_close(window_close_event& e)
	{
		//_running = false;
		return false;
	}

	bool application::_on_window_resize(window_resize_event& e)
	{
		//pGraphics->OnWindowResize();
		return false;
	}

	bool application::_on_key_down(key_down_event& e)
	{
		Input::Instance()->OnKeyDown(e.GetKey());
		return false;
	}
}
