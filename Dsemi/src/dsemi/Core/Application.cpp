// Application.cpp
#include "dspch.h"
#include "dsemi/core/dsemiwindows.h"
#include "dsemi/core/application.h"
#include "dsemi/core/input.h"

#include "dsemi/util/timer.h"
#include "dsemi/util/assert.h"


namespace dsemi {
	application* application::_instance = nullptr;

	application::application()
		:_delta_time(0)
		,_tick_rate_actual(0)
		,_tick_rate_desired(0)
		,_time_per_tick(0)
	{
		if (_instance)
		{
			throw std::exception("Application Instance already exists! Use Application::Get()");
		}
	}

	application::~application()
	{
	}

	void application::init()
	{
		ASSERT(!_instance, "APP: Application Instance already initialized.");

		// WIN32 / WIN64 Console style
		CONSOLE_FONT_INFOEX cfi;
		cfi.cbSize = sizeof(cfi);
		cfi.nFont = 0;
		cfi.dwFontSize.X = 0;
		cfi.dwFontSize.Y = 14;
		cfi.FontFamily = FF_DONTCARE;
		cfi.FontWeight = FW_NORMAL;
		std::wcscpy(cfi.FaceName, L"Consolas");
		SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);

		logger::start_logger();
#ifdef _DEBUG
		logger::set_level(logger::level::LOG_LEVEL_DEBUG);
#else
		logger::set_level(logger::level::LOG_LEVEL_INFO);
#endif
		logger::info("dsemi::Appliation starting logger thread and initializing.");

		_instance = this;

		// Initialize DirectX and Direct3D
		//Direct3D::InitD3D();


		// add loading in a config file for window resolution etc.
		// ^ no dont do that
		// id rather just have the window created in the dev application itself, this should just be a toolbox
		// in other words: decouple the main application from graphics, window, audio, network, etc. systems
		// but do that later on
		_main_wnd = std::make_unique<window>(640, 480, L"Sample window");
		_main_wnd->set_event_callback(BIND_EVENT(application::handle_event));

		// initialize graphics framework
		gfx_core::initialize();

		set_tick_rate(60u);

		//m_pMainCamera = std::make_shared<Camera3D>();

		//GFXResourceManager::InitSingleton();

		//m_pMainWnd->renderContext.SetCamera(m_pMainCamera.get());
		// Run the OnCreate function of client app
		on_create();
	}

	void application::run()
	{
#if 1
		AllocConsole();
		freopen("CONOUT$", "w", stdout);
#endif

		try 
		{
			init();
			while (_running)
			{
				_main_wnd->dispatch_events();
				// Update
				if (do_tick())
				{
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
	}

	void application::update(const float dt)
	{
		on_update(dt);

		Input::ResetMouseDelta();
	}

	bool application::do_tick()
	{
		_delta_time = _update_timer.Peek();

		if (_delta_time >= _time_per_tick)
		{
			unsigned short numTicks = (unsigned short)floor(_delta_time / _time_per_tick); // Get the number of ticks that should have been processed since last frame
			//m_dt -= m_timePerTick * numTicks;
			_update_timer.Mark();

			update(_delta_time);
			return true;
		}
		else
		{
			return false;
		}
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
	}

	// Event Functions
	bool application::_on_window_close(window_close_event& e)
	{
		_running = false;
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