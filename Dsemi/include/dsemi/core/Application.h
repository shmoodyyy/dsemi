// Application.h
#ifndef HEADER_DSEMI_CORE_APPLICATION
#define HEADER_DSEMI_CORE_APPLICATION

#include "dsemi/core/window.h"
#include "dsemi/core/scene.h"

#include "dsemi/events/applicationEvent.h"
#include "dsemi/events/keyboardevent.h"
#include "dsemi/events/mouseevent.h"

#include "Dsemi/graphics/graphics.h"

#include "dsemi/util/assert.h"
#include "dsemi/util/logger.hpp"
#include "dsemi/util/timer.h"

#define WND dsemi::window::get_wnd()

namespace dsemi {
	class application
	{
	public:
		application();
		virtual ~application();
		static inline application& get() 
		{
			ASSERT(_instance, "Attempted to access Application Instance before being initialized!");
			return *_instance; 
		}

		void init();
		virtual void on_create() {};

		void run();
		void shutdown();

		// Application runtime loop functions
	private:
		void do_update(const float dt);
		void do_events();
		void do_render();


		// tick-based updates
	private:
		bool do_tick();

	public:
		void set_tick_rate(unsigned int tickRate) noexcept 
		{
			_tick_rate_desired = tickRate;
			if (tickRate != 0)
				_time_per_tick = 1.0f / (float)tickRate;
		}
		inline unsigned int get_tick_rate() const noexcept { return _tick_rate_desired;	}
		inline float get_tick_rate_actual() const noexcept { return _tick_rate_actual; }


		// custom behaviour for update, event and render calls
	protected:
		virtual void on_update(const float dt) {}
		virtual void on_event(ievent& e) {}
		virtual void on_render() {}
	private:
		unsigned int _tick_rate_desired;
		float        _tick_rate_actual;
		float        _time_per_tick;
		float        _delta_time;
		Timer        _update_timer;


	public: 
		void handle_event(ievent& e);

		// scene management
	public:
		scene* active_scene;

		// event functions
	private:
		bool _on_window_close(window_close_event& e);
		bool _on_window_resize(window_resize_event& e);
		bool _on_key_down(key_down_event& e);

	protected: 
		bool _running = true;
		int _tick_rate = 60;
		
		std::unique_ptr<window> _main_wnd;

	private:
		static application* _instance;
	};

	application* create();
}

#endif