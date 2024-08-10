#include <dsemi/graphics/resources/vertexshader.h>
#include <dsemi/graphics/device.h>

namespace
{
    // i hate microsoft
    wchar_t g_pathBuf[512] = L"shaders/";
    constexpr wchar_t* g_nameStart = g_pathBuf + 8;
}

dsemi::graphics::VertexShader::VertexShader(std::string_view name, std::shared_ptr<VertexLayout> layout)
    : m_layout(layout)
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
    GFX_THROW_FAILED(Device::get().getDxDevice()->CreateVertexShader(
        m_d3d11Blob->GetBufferPointer(),
        m_d3d11Blob->GetBufferSize(),
        nullptr,
        &m_d3d11VertexShader
    ));
    memset(g_nameStart, 0, name.length() + 4);
    delete[] wideStr;

    // 10.08.2024: TODO: actually implement iterators in the layout
    D3D11_INPUT_ELEMENT_DESC* input_elements = new D3D11_INPUT_ELEMENT_DESC[m_layout->size()];
    for (unsigned i = 0; i < m_layout->size(); ++i) {
        auto& elem = m_layout->get(i);
        // todo: pull more of these params from the element (instanced elements)
        input_elements[i].SemanticName = elem.semantic().data();
        input_elements[i].SemanticIndex = 0;
        input_elements[i].Format = ShaderDataType_toDxgi(elem.type());
        input_elements[i].InputSlot = i;
        input_elements[i].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
        input_elements[i].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
        input_elements[i].InstanceDataStepRate = 0;
    }
    GFX_THROW_FAILED(Device::get().getDxDevice()->CreateInputLayout(
        input_elements,
        m_layout->size(),
        m_d3d11Blob->GetBufferPointer(),
        m_d3d11Blob->GetBufferSize(),
        &m_d3d11InputLayout
    ));
    delete[] input_elements;
}

void dsemi::graphics::VertexShader::bind()
{
    Device::get().getContext()->VSSetShader(m_d3d11VertexShader.Get(), nullptr, 0u);
    Device::get().getContext()->IASetInputLayout(m_d3d11InputLayout.Get());
}
