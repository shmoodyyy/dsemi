#pragma once

#include "dsemi/graphics/api_include.h"
#include "dsemi/graphics/bindable.h"

namespace dsemi
{
	namespace graphics
	{
		class ishader : public bindable
		{
		public:
			ishader()
			{};
			virtual ~ishader() = default;


		private:

		};

		class vertex_shader : public ishader
		{
		public:


		private:

		};
	}
}