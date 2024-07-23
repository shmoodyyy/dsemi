// Application.h
#ifndef HEADER_DSEMI_CORE_APPLICATION
#define HEADER_DSEMI_CORE_APPLICATION

#include "dsemi/core/window.h"
#include "dsemi/scene/scene.h"

#include "dsemi/events/applicationEvent.h"
#include "dsemi/events/keyboardevent.h"
#include "dsemi/events/mouseevent.h"

#include "Dsemi/graphics/graphics.h"

#include "dsemi/util/assert.h"
#include "dsemi/util/logger.h"
#include "dsemi/util/timer.h"

#define WND dsemi::window::get_wnd()

namespace dsemi {
	class application
	{
	public:
		application();
		virtual ~application();
		static inline application* get() 
		{
			ASSERT(_instance, "Attempted to access Application Instance before being initialized!");
			return _instance; 
		}

		void init();
		void run();
		void shutdown();

	protected:
		virtual void onInit() {};
		virtual void onDestroy() {};

		// Application runtime loop functions
	private:
		void doUpdate(const float dt);
		void doEvents();
		void doRender();


		// tick-based updates
	private:
		bool doTick();

	public:
		void setTickRate(unsigned int tickRate);
		inline unsigned int getTickRate() const { return m_tickRateDesired;	}
		inline float getTickRateActual() const { return m_tickRateActual; }


		// custom behaviour for update, event and render calls
	protected:
		virtual void onUpdate(const float dt) {}
		virtual void onEvent(ievent& e) {}
		virtual void onRender() {}
	private:
		unsigned int m_tickRateDesired;
		float        m_tickRateActual;
		float        m_msPerTick;
		float        m_deltaTime;
		Timer        m_updateTimer;


	public: 
		void handle_event(ievent& e);

		// scene management
	public:
		scene* m_activeScene;

		// event functions
	private:
		bool onWindowClose(WindowCloseEvent& e);
		bool onWindowResize(WindowResizeEvent& e);
		bool onKeyDown(KeyDownEvent& e);

	protected: 
		bool m_isRunning = true;
		int m_tickRate = 60;
		
		std::unique_ptr<window> m_mainWindow;

	private:
		static application* _instance;
	};
}

#endif
