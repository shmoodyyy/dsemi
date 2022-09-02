// Application.h
#ifndef HEADER_DSEMI_CORE_APPLICATION
#define HEADER_DSEMI_CORE_APPLICATION

#include "dsemi/core/window.h"

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

		// == Update
	public:
		void set_tick_rate(unsigned int tickRate) noexcept {
			_tick_rate_desired = tickRate;
			if (tickRate != 0)
				_time_per_tick = 1.0f / (float)tickRate;
		}
		unsigned int get_tick_rate() const noexcept {
			return _tick_rate_desired;
		}
		float get_tick_rate_actual() const noexcept {
			return _tick_rate_actual;
		}
	protected:
		virtual void on_update(const float dt) {}
	private:
		bool do_tick();
		void update(const float dt);
	private:
		unsigned int _tick_rate_desired;
		float        _tick_rate_actual;
		float        _time_per_tick;
		float        _delta_time;
		Timer        _update_timer;

		// == Render
	protected:
		virtual void on_render() {}

		// == Events
	public: 
		void handle_event(ievent& e);
		virtual void on_event(ievent& e) {};

	private:
		bool _on_window_close(window_close_event& e);
		bool _on_window_resize(window_resize_event& e);
		bool _on_key_down(key_down_event& e);

		// == Member Variables
	protected: 
		bool _running = true;
		int _tick_rate = 60;
		
		// Unique ptrs
		std::unique_ptr<window> _main_wnd;

	private:
		static application* _instance;
	};

	application* create();
}

#endif