#ifndef DSEMI_GRAPHICS_BINDABLE
#define DSEMI_GRAPHICS_BINDABLE

namespace dsemi::graphics
{
    class Bindable
    {
    public:
        virtual ~Bindable() = default;
        virtual void bind() = 0;
	};
}
#endif
