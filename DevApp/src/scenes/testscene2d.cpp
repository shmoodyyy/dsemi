#include "testscene2d.h"

test_scene2d::test_scene2d()
{
	_layer_stack.push_layer(new test_layer_core());
	_layer_stack.push_layer(new test_layer_2dgfx());
}