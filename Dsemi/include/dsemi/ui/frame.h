#ifndef UI_FRAME_BASE_H
#define UI_FRAME_BASE_H

#include "dsemi/ui/element.h"

namespace dsemi {

	class ui_frame : public ui_element
	{
	public:

		virtual void on_update(const float dt) override;
		virtual void on_render(const vector2f& parent_offset) override;

	protected:


	private:


	};

}

#endif