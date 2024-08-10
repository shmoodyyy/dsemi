#include <dsemi/graphics/resources/fragmentshader.h>
#include <dsemi/graphics/device.h>

namespace
{
    // i hate microsoft
    wchar_t g_pathBuf[512] = L"shaders/";
    constexpr wchar_t* g_nameStart = g_pathBuf + 8;
}

dsemi::graphics::FragmentShader::FragmentShader(std::string_view name)
{
    // TODO: log error if name path is too long
    HRESULT hr;
    // read from file
    wchar_t* wideStr = new wchar_t[name.length()+1];
    memset(wideStr, 0, name.length() * sizeof(wchar_t));
    std::mbstowcs(wideStr, name.data(), name.length());
    memcpy(g_nameStart, wideStr, name.length() * sizeof(wchar_t));
    memcpy(g_nameStart + name.length(), L".cso", 4 * sizeof(wchar_t));
    GFX_THROW_FAILED(D3DReadFileToBlob(g_pathBuf, &m_d3d11Blob));
    // create in gpu
    GFX_THROW_FAILED(Device::get().getDxDevice()->CreatePixelShader(
        m_d3d11Blob->GetBufferPointer(),
        m_d3d11Blob->GetBufferSize(),
        nullptr,
        &m_d3d11PixelShader
    ));
    memset(g_nameStart, 0, name.length() + 4);
    delete[] wideStr;
}

void dsemi::graphics::FragmentShader::bind()
{
    Device::get().getContext()->PSSetShader(m_d3d11PixelShader.Get(), nullptr, 0u);
}
