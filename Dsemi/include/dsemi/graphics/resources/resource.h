#ifndef HEADER_DSEMI_GRAPHICS_RESOURCES_RESOUCE
#define HEADER_DSEMI_GRAPHICS_RESOURCES_RESOURCE

namespace dsemi 
{
	namespace graphics 
	{
		class device;

		class iresource {
		public:
			iresource() {};
			virtual ~iresource() { release(); };
			virtual void create(device* device) = 0;
			virtual void release() = 0;
		protected:
			inline device* get_device() { return _device; }
			device* _device;
		};
	}
}

#endif