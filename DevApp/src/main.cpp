#include <dsemi.h>
#include "testLayers/coretestlayer.h"

class DevApp : public dsemi::application
{
public:
	DevApp()
	{
	}

	~DevApp()
	{
	}

	virtual void on_create() override
	{

	}

	virtual void on_event(dsemi::ievent& e) override
	{
		test_layer_core.on_event(e);
	}

private:
	test_layer_core test_layer_core;
};

dsemi::application* dsemi::create()
{
	return new DevApp();
}

int main()
{
	DevApp app;
	app.run();

	return 0;
}