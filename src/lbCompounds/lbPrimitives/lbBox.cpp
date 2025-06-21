#include "lbBox.h"

void lb::Box::Build()
{
	this->vTransformationMatrix = lb::Matrix4f4::Identity();
	this->vColor = { 1.f, 1.f, 1.f, 1.f };

	this->CreateConstBuffer(lb::ConstantBuffer<lb::Matrix4f4, lb::ShaderType::VS>(0));
	this->SetTransformMatrix(this->vTransformationMatrix);

	this->CreateConstBuffer(lb::ConstantBuffer<lb::sColor4f, lb::ShaderType::VS>(1));
	this->SetColor(this->vColor);

	std::vector<lb::VertexDefault3D> vVertices =
	{
		{ -.5,  .5,  .5 },
		{  .5,  .5,  .5 },
		{ -.5, -.5,  .5 },
		{  .5, -.5,  .5 },

		{ -.5,  .5, -.5 },
		{  .5,  .5, -.5 },
		{ -.5, -.5, -.5 },
		{  .5, -.5, -.5 },
	};

	this->CreateVetexBuffer<lb::VertexDefault3D>(vVertices);

	std::vector<unsigned short> vIndices =
	{
		0,1,2,	1,3,2,
		0,4,1,	4,5,1,
		4,6,5,	6,7,5,
		7,6,3,	6,2,3,
		1,5,3,	5,7,3,
		0,2,4,	2,6,4,
	};

	this->CreateIndexBuffer(vIndices);

	this->CreatePixelShader(L"PSDefault3D.cso");

	wrl::ComPtr<ID3DBlob> pBlob = this->CreateVertexShader(L"VSDefault3D.cso");

	const std::vector<D3D11_INPUT_ELEMENT_DESC> vIed = {
		{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 },
	};

	this->CreateInputLayout(vIed, pBlob);

	this->CreateViewport();

	this->CreateDrawRenderTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, vIndices.size());
}

lb::Box::Box()
{
	this->Emplace();
	this->Build();
}