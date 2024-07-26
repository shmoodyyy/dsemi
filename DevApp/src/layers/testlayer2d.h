#ifndef HEADER_DEVAPP_LAYERS_TESTLAYER2D
#define HEADER_DEVAPP_LAYERS_TESTLAYER2D

#include <dsemi/core/layer.h>
#include <dsemi/events/mouseevent.h>
#include <dsemi/events/keyboardevent.h>

namespace dsemi {
	namespace ecs {
		class world;
	}
}

class test_layer_2dgfx : public dsemi::layer 
{
public:
	test_layer_2dgfx(dsemi::ecs::world* entities)
		:layer("TestLayer2DGFX"), _entities(entities)
	{
		_on_create();
	}
	virtual void on_attach() override;

	virtual void on_update(const float dt) override;
	virtual void on_event(dsemi::ievent& e) override;

private:
	void _on_create();
	bool _on_mouse_press(dsemi::mouse_press_event& e);

private:
	dsemi::ecs::world* _entities;
};

#endif
