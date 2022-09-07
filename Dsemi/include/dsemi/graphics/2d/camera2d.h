#ifndef HEADER_DSEMI_GRAPHICS_2D_CAMERA2D
#define HEADER_DSEMI_GRAPHCIS_2D_CAMERA2D

#include "dsemi/math/vector2.h"

namespace dsemi {
	class camera2d
	{
	public:
		camera2d(float scale = 1.0f)
			:_position(0,0), _scale(scale)
		{}
		camera2d(vector2f pos, float scale = 1.0f)
			:_position(pos), _scale(scale)
		{}
		camera2d(float x, float y, float scale = 1.0f)
			:_position(x, y), _scale(scale)
		{}
		~camera2d()
		{}

		inline vector2f position()   const noexcept { return _position; }
		inline float x()             const noexcept { return _position.x; }
		inline float y()             const noexcept { return _position.y; }
		inline float scale()         const noexcept { return _scale; }

		inline void position(vector2f val) noexcept { _position = val; }
		inline void x(float val)           noexcept { _position.x = val; }
		inline void y(float val)           noexcept { _position.y = val; }
		inline void scale(float val)       noexcept { _scale = val; }

	private:
		vector2f _position;
		float _scale;
	};
}

#endif