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
			ishader(device* device) : bindable(device) {}
			virtual ~ishader() = default;

			//virtual void bind() const = 0;


		protected:

		private:

		};

		class vertex_shader : public ishader
		{
		public:
			vertex_shader(device* device) : ishader(device) {}

			virtual void bind() const override;


		private:

		};
	}
}