#ifndef HEADER_DSEMI_GRAPHICS_RESOURCES_RESOUCE
#define HEADER_DSEMI_GRAPHICS_RESOURCES_RESOURCE

namespace dsemi {
	namespace graphics {
		class device;

		class iresource {
		public:
			iresource() = default;
			virtual ~iresource() = default;

			virtual void bind(device& device) const = 0;

		};
	}
}

#endif