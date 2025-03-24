#pragma once

#include "../lbPipelineComponent.h"

namespace lb
{
	class Rasterizer : public lb::IPipelineComponent
	{
	private:
		ID3D11RasterizerState* pRasterizerState;

	public:
		Rasterizer(D3D11_RASTERIZER_DESC v_rasterizerDesc);
		~Rasterizer();
		void Bind() override;
	};
}