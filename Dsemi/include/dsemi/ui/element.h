#ifndef UI_ELEMENT_BASE_H
#define UI_ELEMENT_BASE_H

#include "Dsemi/Math/vector2.h"
#include "dsemi/graphics/colour.h"

namespace dsemi {

	// interface for base ui element
	class ui_element {
	public:
		ui_element();
		~ui_element();

	public:
		virtual void on_update(const float dt) = 0;
		virtual void on_render(const vector2f& parent_offset) = 0;

	public:
		void update(const float dt);
		void render(const vector2f& parent_offset);

		void add_child(ui_element* element);
		const std::vector<ui_element*>& get_children() const noexcept { return _children; }

	public:
		vector2f offset;
		vector2f position;
		vector2f size;
		colour_rgba background_colour;
		colour_rgba outline_colour;
		float outline_thickness;

	protected:
		ui_element* _parent;
		std::vector<ui_element*> _children;
	};

}

#endif