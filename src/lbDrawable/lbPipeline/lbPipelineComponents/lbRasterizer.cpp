#include "lbRasterizer.h"

lb::Rasterizer::Rasterizer(D3D11_RASTERIZER_DESC v_rasterizerDesc)
{
	lb::LB_ENGINE_NAME.GetDevice()->CreateRasterizerState(&v_rasterizerDesc, &this->pRasterizerState);
}

lb::Rasterizer::~Rasterizer()
{
	this->pRasterizerState->Release();
	this->pRasterizerState = nullptr;
}

void lb::Rasterizer::Bind()
{
	lb::LB_ENGINE_NAME.GetContext()->RSSetState(pRasterizerState);
}
