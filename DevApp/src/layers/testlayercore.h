#ifndef HEADER_DEVAPP_LAYERS_TESTLAYERCORE
#define HEADER_DEVAPP_LAYERS_TESTLAYERCORE

#include <dsemi/core/layer.h>
#include <dsemi/events/mouseevent.h>
#include <dsemi/events/keyboardevent.h>
#include <dsemi/events/applicationevent.h>

class test_layer_core : public dsemi::layer
{
public:
	virtual void on_update(const float dt) override;
	virtual void on_event(dsemi::ievent& e) override;

private:
	bool _on_mouse_press(dsemi::window_focus_event& e);
};

#endif