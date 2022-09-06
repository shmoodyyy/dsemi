#ifndef DEV_CORE_TEST_LAYER_H
#define DEV_CORE_TEST_LAYER_H

#include <dsemi/core/layer.h>
#include <dsemi/events/mouseevent.h>
#include <dsemi/events/keyboardevent.h>

class test_layer_core : public dsemi::layer
{
public:
	virtual void on_update(const float dt) override;
	virtual void on_event(dsemi::ievent& e) override;
	virtual void on_render(const float dt) override;

private:
	bool _on_mouse_press(dsemi::mouse_press_event& e);
};

#endif