#include "lbDepthStencil.h"

lb::DepthStencil::DepthStencil(D3D11_DEPTH_STENCIL_DESC v_depthStencilDesc, int v_stencilValue)
	: vStencilValue(v_stencilValue) 
{
	lb::LB_ENGINE_NAME.GetDevice()->CreateDepthStencilState(&v_depthStencilDesc, &this->pDepthStencilState);
}

lb::DepthStencil::~DepthStencil()
{
	this->pDepthStencilState->Release();
	this->pDepthStencilState = nullptr;
}

void lb::DepthStencil::Bind()
{
	lb::LB_ENGINE_NAME.GetContext()->OMSetDepthStencilState(this->pDepthStencilState, vStencilValue);
}
