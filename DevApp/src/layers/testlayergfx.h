#ifndef HEADER_DEVAPP_LAYERS_TESTLAYERGFX
#define HEADER_DEVAPP_LAYERS_TESTLAYERGFX

#include <dsemi/graphics/graphics.h>
#include <dsemi/core/layer.h>

class test_layer_gfx : public dsemi::layer
{
public:
	virtual void on_attach() override;

	virtual void on_update(const float dt) override;
	virtual void on_event(dsemi::ievent& e) override;

private:

};

#endif