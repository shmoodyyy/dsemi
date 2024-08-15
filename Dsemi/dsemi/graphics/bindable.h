#ifndef DSEMI_GRAPHICS_BINDABLE
#define DSEMI_GRAPHICS_BINDABLE

namespace dsemi::graphics
{
    enum class BindMode : unsigned char
    {
        bind_static,
        bind_dynamic
    };

    class Bindable
    {
    public:
        virtual ~Bindable() = default;
        virtual void bind() = 0;
	};
}
#endif
