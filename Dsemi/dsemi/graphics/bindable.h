#pragma once

namespace dsemi
{
	namespace graphics
	{
		class device;

		class bindable
		{
		public:
			bindable(device* parent_device) : _device(parent_device) {} virtual ~bindable() = default;

			inline virtual void bind() const noexcept = 0;
		protected:
			inline device* _get_device() const noexcept { return _device; }
		private:
			device* _device;
		};
	}
}