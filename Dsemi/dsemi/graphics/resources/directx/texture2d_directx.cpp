#include <dsemi/graphics/resources/texture2d.h>
#include <dsemi/graphics/device.h>

dsemi::graphics::Texture2D::Texture2D(unsigned width, unsigned height)
{
    D3D11_TEXTURE2D_DESC td = {0};
    td.Width = width;
    td.Height = height;
    td.MipLevels = 1;
    td.ArraySize = 1;
    td.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    td.SampleDesc.Count = 1;
    td.Usage = D3D11_USAGE_DEFAULT;
    td.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    td.MiscFlags = 0;
    HRESULT hr;
    GFX_THROW_FAILED(Device::get().get_dx_device()->CreateTexture2D(
        &td,
        nullptr,
        &m_texture2d
    ));
}

dsemi::graphics::Texture2D::Texture2D(ID3D11Texture2D* dxTexture)
    : m_texture2d(dxTexture)
{
}

dsemi::graphics::Texture2D::~Texture2D()
{
}


