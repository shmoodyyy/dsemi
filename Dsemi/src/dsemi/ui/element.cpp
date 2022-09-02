#include "dspch.h"
#include "dsemi/ui/element.h"

namespace dsemi {

	ui_element::ui_element()
	{

	}

	ui_element::~ui_element()
	{
		for (auto child : _children)
		{
			delete child;
		}
	}

	void ui_element::add_child(ui_element* element)
	{
		element->_parent = this;
		_children.push_back(element);
	}

	void ui_element::update(const float dt)
	{
		on_update(dt);
		for (auto child : _children)
		{
			child->on_update(dt);
		}
	}

	void ui_element::render(const vector2f& parent_offset)
	{
		on_render(parent_offset);
		for (auto child : _children)
		{
			child->render(parent_offset + (position + offset));
		}
	}

}