#ifndef HEADER_DSEMI_GRAPHICS_RESOURCES_RESOUCE
#define HEADER_DSEMI_GRAPHICS_RESOURCES_RESOURCE

namespace dsemi 
{
	namespace graphics 
	{
		class Device;

		class iresource {
		public:
			iresource() {};
			virtual ~iresource() { release(); };
			virtual void create(Device* device) = 0;
			virtual void release() {};
		protected:
			inline Device* get_device() { return _device; }
			Device* _device;
		};
	}
}

#endif
