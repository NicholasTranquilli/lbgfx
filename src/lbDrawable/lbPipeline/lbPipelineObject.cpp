#include "lbPipelineObject.h"

void lb::IPipelineObject::Emplace() 
{
	lb::LB_ENGINE_NAME.AddDrawable(this);
}

void lb::IPipelineObject::Draw()
{
	for (auto& v_elem : this->vConstantComponents)
		v_elem->Bind();
	for (auto& v_elem : this->vPipelineComponents)
		v_elem->Bind();
}

void lb::IPipelineObject::Update()
{
	for (auto& v_elem : this->vConstantComponents)
		v_elem->Update();
}

void lb::IPipelineObject::Delete()
{
	lb::LB_ENGINE_NAME.RemoveDrawable(this);
	this->vPipelineComponents.clear();
	this->vConstantComponents.clear();
}

void lb::IPipelineObject::Rebuild()
{
	this->Delete();
	this->Build();
}

void lb::IPipelineObject::CreateCustomBindable(lb::CustomBindable::CustomBindingFn p_bindFn)
{
	this->vPipelineComponents.push_back(std::make_shared<lb::CustomBindable>(p_bindFn));
}

void lb::IPipelineObject::CreateIndexBuffer(std::vector<unsigned short>& v_indices)
{
	this->vPipelineComponents.push_back(std::make_shared<lb::IndexBuffer>(lb::IndexBuffer(v_indices)));
}

void lb::IPipelineObject::CreatePixelShader(std::wstring v_fileName)
{
	this->vPipelineComponents.push_back(std::make_shared<lb::PixelShader>(lb::PixelShader(v_fileName)));
}

void lb::IPipelineObject::CreateInputLayout(const std::vector<D3D11_INPUT_ELEMENT_DESC>& v_ied, wrl::ComPtr<ID3DBlob>& p_blob)
{
	this->vPipelineComponents.push_back(std::make_shared<lb::InputLayout>(lb::InputLayout(v_ied, p_blob)));
}

void lb::IPipelineObject::CreateViewport()
{
	this->vPipelineComponents.push_back(std::make_shared<lb::Viewport>(lb::Viewport()));
}

void lb::IPipelineObject::CreateDrawRenderTopology(D3D_PRIMITIVE_TOPOLOGY v_primitiveTopology, UINT v_indexCount, UINT v_indexStart)
{
	this->vPipelineComponents.push_back(std::make_shared<lb::DrawRenderTopology>(lb::DrawRenderTopology(v_primitiveTopology, v_indexCount, v_indexStart)));
}

wrl::ComPtr<ID3DBlob> lb::IPipelineObject::CreateVertexShader(std::wstring v_fileName)
{
	lb::VertexShader vVertexShader(v_fileName);
	this->vPipelineComponents.push_back(std::make_shared<lb::VertexShader>(vVertexShader));
	return vVertexShader.GetBlob();
}

void lb::IPipelineObject::CreateDepthStencilBuffer(D3D11_DEPTH_STENCIL_DESC v_depthStencilDesc, int v_stencilValue)
{
	this->vPipelineComponents.push_back(std::make_shared<lb::DepthStencil>(v_depthStencilDesc, v_stencilValue));
}

void lb::IPipelineObject::CreateRasterState(D3D11_RASTERIZER_DESC v_rasterizerDesc)
{
	this->vPipelineComponents.push_back(std::make_shared<lb::Rasterizer>(v_rasterizerDesc));
}

