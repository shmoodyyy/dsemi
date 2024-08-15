#include <dsemi/graphics/resources/constantbuffer.h>
#include <dsemi/graphics/device.h>

dsemi::graphics::ConstantBuffer::ConstantBuffer(unsigned bindSlot, void* data, size_t size, size_t stride, BindType bindType)
    : m_bindSlot(bindSlot)
    , m_bindType(bindType)
    , m_data(nullptr)
    , m_size(size)
    , m_stride(stride)
{
    m_data = new char[size];
    memcpy(m_data, data, size);

    HRESULT hr;
    D3D11_BUFFER_DESC vmcbd   = {};
    vmcbd.ByteWidth           = size;
    vmcbd.Usage               = D3D11_USAGE_DYNAMIC;
    vmcbd.BindFlags           = D3D11_BIND_CONSTANT_BUFFER;
    vmcbd.CPUAccessFlags      = D3D11_CPU_ACCESS_WRITE;
    vmcbd.MiscFlags           = 0u;
    vmcbd.StructureByteStride = m_stride;

    D3D11_SUBRESOURCE_DATA vmsrd = {};
    vmsrd.pSysMem = m_data;

    GFX_THROW_FAILED(Device::get().getDxDevice()->CreateBuffer(&vmcbd, &vmsrd, &m_d3d11Buffer));
}

dsemi::graphics::ConstantBuffer::~ConstantBuffer()
{
    if (m_data)
        delete[] reinterpret_cast<char*>(m_data); // lol
}

void dsemi::graphics::ConstantBuffer::bind()
{
    switch (m_bindType) {
    case BindType::cb_bindVertexShader:
        Device::get().getContext()->VSSetConstantBuffers(m_bindSlot, 1u, m_d3d11Buffer.GetAddressOf());
        break;
    case BindType::cb_bindFragmentShader:
        Device::get().getContext()->PSSetConstantBuffers(m_bindSlot, 1u, m_d3d11Buffer.GetAddressOf());
        break;
    default:
        break;
    }
}

void dsemi::graphics::ConstantBuffer::updateResource()
{
    D3D11_MAPPED_SUBRESOURCE msr;
    Device::get().getContext()->Map(m_d3d11Buffer.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &msr);
    memcpy(msr.pData, m_data, m_size);
    Device::get().getContext()->Unmap(m_d3d11Buffer.Get(), 0u);
}
