#include <dsemi.h>
#include "scenes/testscene2d.h"

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
		active_scene = &test_scene;
	}

	virtual void on_event(dsemi::ievent& e) override
	{
		//test_layer_core.on_event(e);
	}

private:
	test_scene2d test_scene;
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