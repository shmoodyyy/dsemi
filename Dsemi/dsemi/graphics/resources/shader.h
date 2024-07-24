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
			ishader(Device* device) : bindable(device) {}
			virtual ~ishader() = default;

			//virtual void bind() const = 0;


		protected:

		private:

		};

		class vertex_shader : public ishader
		{
		public:
			vertex_shader(Device* device) : ishader(device) {}

			virtual void bind() const override;


		private:

		};
	}
}
