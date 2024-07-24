#pragma once

namespace dsemi
{
	namespace graphics
	{
		class Device;

		class bindable
		{
		public:
			bindable(Device* parent_device) : _device(parent_device) {} virtual ~bindable() = default;

			inline virtual void bind() const = 0;
		protected:
			inline Device* _get_device() const { return _device; }
		private:
			Device* _device;
		};
	}
}
