#include "testscene2d.h"

TestScene2D::TestScene2D()
{
	_layer_stack.push_layer(new test_layer_core());
	_layer_stack.push_layer(new test_layer_2dgfx(&_entities));
}
