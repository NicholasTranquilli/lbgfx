#include "lbIndexBuffer.h"

lb::IndexBuffer::IndexBuffer(std::vector<unsigned short>& v_indices)
	: vIndices(v_indices)
{
	D3D11_BUFFER_DESC vIbd = {};
	vIbd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	vIbd.Usage = D3D11_USAGE_DEFAULT;
	vIbd.CPUAccessFlags = 0u;
	vIbd.MiscFlags = 0u;
	vIbd.ByteWidth = sizeof(unsigned short) * this->vIndices.size();
	vIbd.StructureByteStride = sizeof(unsigned short);

	D3D11_SUBRESOURCE_DATA vIsd = {};
	vIsd.pSysMem = this->vIndices.data();
	lb::LB_ENGINE_NAME.GetDevice()->CreateBuffer(&vIbd, &vIsd, &this->pIndexBuffer);
}

void lb::IndexBuffer::Bind()
{
	lb::LB_ENGINE_NAME.GetContext()->IASetIndexBuffer(this->pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);
}
