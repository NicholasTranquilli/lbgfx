#include "lbRect.h"

void lb::Rect::Build()
{
	this->vTransformationMatrix = lb::Matrix4f4::Identity();
	this->vColor = { 1.f, 1.f, 1.f, 1.f };

	this->CreateConstBuffer(lb::ConstantBuffer<lb::Matrix4f4, lb::ShaderType::VS>(0));
	this->SetTransformMatrix(this->vTransformationMatrix);

	this->CreateConstBuffer(lb::ConstantBuffer<lb::sColor4f, lb::ShaderType::VS>(1));
	this->SetColor(this->vColor);

	std::vector<lb::VertexDefaultUI> vVertices =
	{
		{  0.5f, 0.5f },
		{  0.5f,-0.5f },
		{ -0.5f,-0.5f },
		{ -0.5f, 0.5f },
	};

	this->CreateVetexBuffer<lb::VertexDefaultUI>(vVertices);

	std::vector<unsigned short> vIndices =
	{
		0,1,3,
		1,2,3,
	};

	this->CreateIndexBuffer(vIndices);

	this->CreatePixelShader(L"PSDefaultUI.cso");

	wrl::ComPtr<ID3DBlob> pBlob = this->CreateVertexShader(L"VSDefaultUI.cso");

	const std::vector<D3D11_INPUT_ELEMENT_DESC> vIed = {
		{ "Position",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 },
	};

	this->CreateInputLayout(vIed, pBlob);

	this->CreateViewport();

	this->CreateDrawRenderTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, vIndices.size());
}

lb::Rect::Rect()
{
	this->Emplace();
	this->Build();
}
