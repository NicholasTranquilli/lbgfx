#pragma once

#include "../lbPipelineComponent.h"

namespace lb
{
	class IConstantBufferBase : public lb::IPipelineComponent
	{
	public:
		virtual void Bind() override = 0;
		virtual void Update() = 0;
		virtual ShaderType GetType() = 0;
		virtual unsigned int GetSlot() = 0;
	};

	template <typename _T, ShaderType _CBUFF>
	class ConstantBuffer : public lb::IConstantBufferBase
	{
	private:
		wrl::ComPtr<ID3D11Buffer> pConstBuffer;
		_T vShaderConstData;
		unsigned int vSlot;

	public:
		static_assert(_CBUFF > ShaderType::SHADER_INVALID && _CBUFF < ShaderType::SHADER_MAX, "Invalid shader type");

		ConstantBuffer(unsigned int v_slot = 0);
		ConstantBuffer(_T v_shaderConstData, unsigned int v_slot = 0);

		inline wrl::ComPtr<ID3D11Buffer>& GetData();

		inline void Bind() override;
		inline void Update() override;
		inline ShaderType GetType() override;
		inline unsigned int GetSlot() override;
		inline void SetData(_T v_shaderConstData);
	};
}

template<typename _T, lb::ShaderType _CBUFF>
inline lb::ConstantBuffer<_T, _CBUFF>::ConstantBuffer(unsigned int v_slot)
	: vSlot(v_slot)
{
	D3D11_BUFFER_DESC vBufferDesc;
	vBufferDesc.ByteWidth = sizeof(_T);
	vBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	vBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vBufferDesc.MiscFlags = 0;
	vBufferDesc.StructureByteStride = 0;

	lb::LB_ENGINE_NAME.GetDevice()->CreateBuffer(&vBufferDesc, NULL, &pConstBuffer);
}

template <typename _T, lb::ShaderType _CBUFF>
inline lb::ConstantBuffer<_T, _CBUFF>::ConstantBuffer(_T v_shaderConstData, unsigned int v_slot)
	: vShaderConstData(v_shaderConstData), vSlot(v_slot)
{
	D3D11_BUFFER_DESC vBufferDesc;
	vBufferDesc.ByteWidth = sizeof(_T);
	vBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	vBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vBufferDesc.MiscFlags = 0;
	vBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vSubData;
	vSubData.pSysMem = &vShaderConstData;
	vSubData.SysMemPitch = 0;
	vSubData.SysMemSlicePitch = 0;

	lb::LB_ENGINE_NAME.GetDevice()->CreateBuffer(&vBufferDesc, &vSubData, &pConstBuffer);
}

template<typename _T, lb::ShaderType _CBUFF>
inline wrl::ComPtr<ID3D11Buffer>& lb::ConstantBuffer<_T, _CBUFF>::GetData()
{
	return this->pConstBuffer;
}

template <typename _T, lb::ShaderType _CBUFF>
inline void lb::ConstantBuffer<_T, _CBUFF>::Bind()
{
	if (_CBUFF == lb::ShaderType::VS)
		lb::LB_ENGINE_NAME.GetContext()->VSSetConstantBuffers(this->vSlot, 1, pConstBuffer.GetAddressOf());
	else if (_CBUFF == lb::ShaderType::PS)
		lb::LB_ENGINE_NAME.GetContext()->PSSetConstantBuffers(this->vSlot, 1, pConstBuffer.GetAddressOf());
}

template<typename _T, lb::ShaderType _CBUFF>
inline void lb::ConstantBuffer<_T, _CBUFF>::Update()
{
	D3D11_MAPPED_SUBRESOURCE vMappedSub;

	lb::LB_ENGINE_NAME.GetContext()->Map(
		this->pConstBuffer.Get(),
		0u,
		D3D11_MAP_WRITE_DISCARD,
		0u,
		&vMappedSub
	);

	memcpy(vMappedSub.pData, &vShaderConstData, sizeof(vShaderConstData));
	lb::LB_ENGINE_NAME.GetContext()->Unmap(this->pConstBuffer.Get(), 0u);
}

template<typename _T, lb::ShaderType _CBUFF>
inline lb::ShaderType lb::ConstantBuffer<_T, _CBUFF>::GetType()
{
	return _CBUFF;
}

template<typename _T, lb::ShaderType _CBUFF>
inline unsigned int lb::ConstantBuffer<_T, _CBUFF>::GetSlot()
{
	return this->vSlot;
}

template<typename _T, lb::ShaderType _CBUFF>
inline void lb::ConstantBuffer<_T, _CBUFF>::SetData(_T v_shaderConstData)
{
	this->vShaderConstData = v_shaderConstData;
}
