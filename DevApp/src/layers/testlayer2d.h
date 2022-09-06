#ifndef HEADER_DEVAPP_TESTLAYERS_GFX_2D
#define HEADER_DEVAPP_TESTLAYERS_GFX_2D

#include <dsemi/core/layer.h>
#include <dsemi/events/mouseevent.h>
#include <dsemi/events/keyboardevent.h>

class test_layer_2dgfx : public dsemi::layer 
{
public:
	test_layer_2dgfx()
		:layer("TestLayer2DGFX")
	{
		_on_create();
	}
	virtual void on_attach() override;

	virtual void on_update(const float dt) override;
	virtual void on_event(dsemi::ievent& e) override;
	virtual void on_render(const float dt) override;

private:
	void _on_create();
	bool _on_mouse_press(dsemi::mouse_press_event& e);
};

#endif