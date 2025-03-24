#pragma once

#include "../lbPipelineComponent.h"

namespace lb
{
	class DepthStencil : public lb::IPipelineComponent
	{
	private:
		ID3D11DepthStencilState* pDepthStencilState; // CLASS MANAGED
		int vStencilValue;

	public:
		DepthStencil(D3D11_DEPTH_STENCIL_DESC v_depthStencilDesc, int v_stencilValue);
		~DepthStencil();
		void Bind() override;
	};
}