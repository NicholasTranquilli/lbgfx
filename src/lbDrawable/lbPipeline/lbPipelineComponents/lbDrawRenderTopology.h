#pragma once

#include "../lbPipelineComponent.h"

namespace lb
{
	class DrawRenderTopology : public lb::IPipelineComponent
	{
	private:
		D3D_PRIMITIVE_TOPOLOGY vPrimitiveTopology;
		UINT vCount;
		UINT vStart;

	public:
		DrawRenderTopology(D3D_PRIMITIVE_TOPOLOGY v_primitiveTopology, UINT v_count, UINT v_start = 0);
		void Bind() override;
	
		enum { COUNT_BY_VERTEX = -1 };
	};
}