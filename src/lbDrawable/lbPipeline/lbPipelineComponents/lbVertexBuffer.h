#pragma once

#include <vector>
#include "../lbPipelineComponent.h"

namespace lb
{
	template <typename _T>
	class VertexBuffer : public lb::IPipelineComponent
	{
	private:
		std::vector<_T> vVertices;
		wrl::ComPtr<ID3D11Buffer> pVertexBuffer;

	public:
		inline VertexBuffer(std::vector<_T> v_vertices);
		inline wrl::ComPtr<ID3D11Buffer>& GetVetexBuffer();
		inline void Bind() override;
	};
}

template<typename _T>
inline lb::VertexBuffer<_T>::VertexBuffer(std::vector<_T> v_vertices)
	: vVertices(v_vertices)
{
	D3D11_BUFFER_DESC vBd = {};
	vBd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vBd.Usage = D3D11_USAGE_DEFAULT;
	vBd.CPUAccessFlags = 0u;
	vBd.MiscFlags = 0u;
	vBd.ByteWidth = sizeof(_T) * this->vVertices.size();
	vBd.StructureByteStride = sizeof(_T);

	D3D11_SUBRESOURCE_DATA vSd = {};
	vSd.pSysMem = this->vVertices.data();
	lb::LB_ENGINE_NAME.GetDevice()->CreateBuffer(&vBd, &vSd, &this->pVertexBuffer);
}

template<typename _T>
inline wrl::ComPtr<ID3D11Buffer>& lb::VertexBuffer<_T>::GetVetexBuffer()
{
	return this->pVertexBuffer;
}

template<typename _T>
inline void lb::VertexBuffer<_T>::Bind()
{
	const UINT vStride = sizeof(_T);
	const UINT vOffset = 0u;
	lb::LB_ENGINE_NAME.GetContext()->IASetVertexBuffers(0u, 1u, this->GetVetexBuffer().GetAddressOf(), &vStride, &vOffset);
}
