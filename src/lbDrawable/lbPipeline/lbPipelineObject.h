#pragma once

#include "lbPipelineComponents/lbVertexBuffer.h"
#include "lbPipelineComponents/lbViewport.h"
#include "lbPipelineComponents/lbIndexBuffer.h"
#include "lbPipelineComponents/lbPixelShader.h"
#include "lbPipelineComponents/lbVertexShader.h"
#include "lbPipelineComponents/lbInputLayout.h"
#include "lbPipelineComponents/lbDrawRenderTopology.h"
#include "lbPipelineComponents/lbViewPort.h"
#include "lbPipelineComponents/lbConstantBuffer.h"
#include "lbPipelineComponents/lbDepthStencil.h"
#include "lbPipelineComponents/lbRasterizer.h"
#include "lbPipelineComponents/lbCustomBindable.h"

namespace lb 
{
	class IPipelineObject : public lb::IDrawable
	{
	private:
		std::vector<std::shared_ptr<lb::IPipelineComponent>> vPipelineComponents;
		std::vector<std::shared_ptr<lb::IConstantBufferBase>> vConstantComponents;

	protected:
		virtual void Build() = 0;

		void Emplace();

		void CreateCustomBindable(lb::CustomBindable::CustomBindingFn p_bindFn);
		void CreateIndexBuffer(std::vector<unsigned short>& v_indices);
		void CreatePixelShader(std::wstring v_fileName);
		void CreateInputLayout(const std::vector<D3D11_INPUT_ELEMENT_DESC>& v_ied, wrl::ComPtr<ID3DBlob>& p_blob);
		void CreateViewport();
		void CreateDrawRenderTopology(D3D_PRIMITIVE_TOPOLOGY v_primitiveTopology, UINT v_indexCount, UINT v_indexStart = 0);
		wrl::ComPtr<ID3DBlob> CreateVertexShader(std::wstring v_fileName);
		template <typename _T>
		inline void CreateVetexBuffer(std::vector<_T>& v_vertices);
		template <typename _T, lb::ShaderType _CBUFF>
		inline void CreateConstBuffer(lb::ConstantBuffer<_T, _CBUFF> v_cbuff);
		void CreateDepthStencilBuffer(D3D11_DEPTH_STENCIL_DESC v_depthStencilDesc, int v_stencilValue);
		void CreateRasterState(D3D11_RASTERIZER_DESC v_rasterizerDesc);

	public:
		void Draw() override;
		void Update() override;
		void Delete();
		void Rebuild();
		
		template <typename _T, lb::ShaderType _CBUFF>
		inline std::shared_ptr<lb::ConstantBuffer<_T, _CBUFF>> GetConstBuffer(unsigned int v_slot = 0);
	};
}

template<typename _T>
inline void lb::IPipelineObject::CreateVetexBuffer(std::vector<_T>& v_vertices)
{
	this->vPipelineComponents.push_back(std::make_shared<lb::VertexBuffer<_T>>(lb::VertexBuffer<_T>(v_vertices)));
}

template<typename _T, lb::ShaderType _CBUFF>
inline void lb::IPipelineObject::CreateConstBuffer(lb::ConstantBuffer<_T, _CBUFF> v_cbuff)
{
	this->vConstantComponents.push_back(std::make_shared<lb::ConstantBuffer<_T, _CBUFF>>(v_cbuff));
}

template<typename _T, lb::ShaderType _CBUFF>
inline std::shared_ptr<lb::ConstantBuffer<_T, _CBUFF>> lb::IPipelineObject::GetConstBuffer(unsigned int v_slot)
{
	for (auto& v_elem : this->vConstantComponents)
		if (v_elem->GetType() == _CBUFF && v_elem->GetSlot() == v_slot)
			return std::dynamic_pointer_cast<lb::ConstantBuffer<_T, _CBUFF>>(v_elem);

	return nullptr;
}
