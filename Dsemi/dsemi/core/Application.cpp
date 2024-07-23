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
#include <iostream>
#include <string_view>
using namespace std::literals;

namespace dsemi {
	application* application::_instance = nullptr;

	application::application()
		: m_deltaTime(0)
		, m_tickRateActual(0)
		, m_tickRateDesired(0)
		, m_msPerTick(0)
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
		onDestroy();
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

		setTickRate(60u);

		//m_pMainCamera = std::make_shared<Camera3D>();

		//GFXResourceManager::InitSingleton();

		//m_pMainWnd->renderContext.SetCamera(m_pMainCamera.get());
		
		// run additional intialization routine
		onInit();
		LOG_DEBUG(L"dsemi::appliation done initializing.");
	}

	// looping section of the application handling update, event handling and render calls
	void application::run()
	{
		try 
		{
			MSG msg = {};
			while (m_isRunning)
			{
				if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
				{
					// todo: some sort of event manager/handler separation?
					TranslateMessage(&msg);
					DispatchMessage(&msg);
					// TODO: replace with application-level message handling
					//_main_wnd->dispatch_events();
					doEvents();
				}

				// Update
				if (doTick())
				{
					doRender();
					/*m_pMainWnd->renderContext.NewFrame();
					m_pMainWnd->renderContext.Present();*/
				}
			}
			shutdown();
		}
		catch (dsemi_exception* e)
		{
            std::cout << e->what() << '\n' << e->type() << '\n';
			display_except(e->what(), e->type());
		}
		catch (std::exception& e)
		{
            std::cout << e.what() << '\n' << "std Exception" << '\n';
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

	void application::doUpdate(const float dt)
	{
		onUpdate(dt);
		m_activeScene->handle_update(dt);

		Input::ResetMouseDelta();
	}

	bool application::doTick()
	{
		m_deltaTime = m_updateTimer.Peek();

		if (m_deltaTime >= m_msPerTick)
		{	
			unsigned short numTicks = (unsigned short)std::floor(m_deltaTime / m_msPerTick); // Get the number of ticks that should have been processed since last frame
			//m_dt -= m_timePerTick * numTicks;
			m_updateTimer.Mark();

			doUpdate(m_deltaTime);
			//active_scene->handle_render(_delta_time);
			return true;
		}
		else
		{
			return false;
		}
	}

	void application::setTickRate(unsigned int tickRate)
	{
		m_tickRateDesired = tickRate;
		if (tickRate != 0)
			m_msPerTick = 1.0f / (float)tickRate;
	}

	void application::doEvents()
	{
	}

	void application::doRender()
	{
	}

	// Event Handler
	void application::handle_event(ievent& e)
	{
		event_dispatcher dispatcher(e);
		dispatcher.dispatch<WindowCloseEvent>(BIND_EVENT(application::onWindowClose));
		dispatcher.dispatch<WindowResizeEvent>(BIND_EVENT(application::onWindowResize));
		dispatcher.dispatch<KeyDownEvent>(BIND_EVENT(application::onKeyDown));

		if (!e.handled) {
			onEvent(e);
		}

		m_activeScene->handle_event(e);
	}

	// Event Functions
	bool application::onWindowClose(WindowCloseEvent& e)
	{
		//_running = false;
		return false;
	}

	bool application::onWindowResize(WindowResizeEvent& e)
	{
		//pGraphics->OnWindowResize();
		return false;
	}

	bool application::onKeyDown(KeyDownEvent& e)
	{
		Input::Instance()->OnKeyDown(e.GetKey());
		return false;
	}
}
