#pragma once

namespace dsemi
{
	namespace graphics
	{
		class Device;

		class bindable
		{
		public:
			bindable(Device* parent_device) : m_device(*parent_device) {} virtual ~bindable() = default;

			inline virtual void bind() const = 0;
		protected:
			inline Device& getDevice() const { return m_device; }
		private:
			Device& m_device;
		};
	}
}
