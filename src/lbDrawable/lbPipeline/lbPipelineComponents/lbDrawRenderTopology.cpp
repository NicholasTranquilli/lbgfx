#include "lbDrawRenderTopology.h"

lb::DrawRenderTopology::DrawRenderTopology(D3D_PRIMITIVE_TOPOLOGY v_primitiveTopology, UINT v_count, UINT v_start)
	: vPrimitiveTopology(v_primitiveTopology), vCount(v_count), vStart(v_start) { }


void lb::DrawRenderTopology::Bind()
{
	lb::LB_ENGINE_NAME.GetContext()->IASetPrimitiveTopology(this->vPrimitiveTopology);

	lb::LB_ENGINE_NAME.GetContext()->OMSetRenderTargets(1u, lb::LB_ENGINE_NAME.GetTarget().GetAddressOf(), lb::LB_ENGINE_NAME.GetDepthStencilView().Get());

	if (this->vStart != lb::DrawRenderTopology::COUNT_BY_VERTEX)
		lb::LB_ENGINE_NAME.GetContext()->DrawIndexed(this->vCount, this->vStart, 0u);
	else
		lb::LB_ENGINE_NAME.GetContext()->Draw(this->vCount, 0u);
}

