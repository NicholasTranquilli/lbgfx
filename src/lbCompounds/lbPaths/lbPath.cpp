#include "lbPath.h"

void lb::Path::Build()
{
	this->vTransformationMatrix = lb::Matrix4f4::Identity();
	this->vColor = { 1.f, 1.f, 1.f, 1.f };

	this->CreateConstBuffer(lb::ConstantBuffer<lb::Matrix4f4, lb::ShaderType::VS>(0));
	this->SetTransformMatrix(this->vTransformationMatrix);

	this->CreateConstBuffer(lb::ConstantBuffer<lb::sColor4f, lb::ShaderType::VS>(1));
	this->SetColor(this->vColor);

	this->CreateVetexBuffer<lb::VertexDefaultUI>(vVertices);

	this->CreateIndexBuffer(vIndices);

	this->CreatePixelShader(L"PSDefaultUI.cso");

	wrl::ComPtr<ID3DBlob> pBlob = this->CreateVertexShader(L"VSDefaultUI.cso");

	const std::vector<D3D11_INPUT_ELEMENT_DESC> vIed = {
		{ "Position",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 },
	};
	this->CreateInputLayout(vIed, pBlob);

	D3D11_RASTERIZER_DESC rasterizerDesc = {};
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;  
	rasterizerDesc.CullMode = D3D11_CULL_NONE;   
	rasterizerDesc.FrontCounterClockwise = FALSE;
	rasterizerDesc.DepthClipEnable = TRUE;       
	rasterizerDesc.ScissorEnable = FALSE;        
	rasterizerDesc.MultisampleEnable = FALSE;    
	rasterizerDesc.AntialiasedLineEnable = FALSE;
	rasterizerDesc.DepthBias = 0;
	rasterizerDesc.SlopeScaledDepthBias = 0.0f;
	rasterizerDesc.DepthBiasClamp = 0.0f;
	this->CreateRasterState(rasterizerDesc);

	this->CreateViewport();

	// Stencil Pass 1
	D3D11_DEPTH_STENCIL_DESC stencilIncrementDesc = {};
	stencilIncrementDesc.DepthEnable = FALSE;
	stencilIncrementDesc.StencilEnable = TRUE;
	stencilIncrementDesc.StencilReadMask = 0xFF;
	stencilIncrementDesc.StencilWriteMask = 0xFF;

	// Always pass stencil test, increment on pass
	stencilIncrementDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	stencilIncrementDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR;
	stencilIncrementDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	stencilIncrementDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;

	stencilIncrementDesc.BackFace = stencilIncrementDesc.FrontFace;
	this->CreateDepthStencilBuffer(stencilIncrementDesc, 0);

	// Blend state (MEMORY LEAK HERE, TODO: ADD BLEND STATE BINDABLE)
	this->CreateCustomBindable([]() {
		D3D11_RENDER_TARGET_BLEND_DESC blendDesc = {};
		blendDesc.RenderTargetWriteMask = 0;

		D3D11_BLEND_DESC blendStateDesc = {};
		blendStateDesc.RenderTarget[0] = blendDesc;

		ID3D11BlendState* noColorWriteBlendState = nullptr;
		lb::LB_ENGINE_NAME.GetDevice()->CreateBlendState(&blendStateDesc, &noColorWriteBlendState);
		lb::LB_ENGINE_NAME.GetContext()->OMSetBlendState(noColorWriteBlendState, nullptr, 0xffffffff);
	});

	// First Draw
	this->CreateDrawRenderTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, vIndices.size());

	// Stencil Pass 2
	D3D11_DEPTH_STENCIL_DESC stencilTestDesc = {};
	stencilTestDesc.DepthEnable = FALSE;
	stencilTestDesc.StencilEnable = TRUE;

	stencilTestDesc.StencilReadMask = 0x1;
	stencilTestDesc.StencilWriteMask = 0x0;

	stencilTestDesc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;
	stencilTestDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	stencilTestDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	stencilTestDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	stencilTestDesc.BackFace = stencilTestDesc.FrontFace;
	this->CreateDepthStencilBuffer(stencilTestDesc, 1);

	// Blend state (MEMORY LEAK HERE, TODO: ADD BLEND STATE BINDABLE)
	this->CreateCustomBindable([]() {
		float blendFactor[4] = { 0, 0, 0, 0 };
		lb::LB_ENGINE_NAME.GetContext()->OMSetBlendState(nullptr, blendFactor, 0xffffffff);
	});

	// Final Draw
	this->CreateDrawRenderTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, vIndices.size());
}

lb::Path::Path(char c, std::vector<lb::VertexDefaultUI> v_vertices, std::vector<unsigned short> v_indices)
	: vVertices(v_vertices), vIndices(v_indices)
{
	this->Emplace();
	this->Build();
}
