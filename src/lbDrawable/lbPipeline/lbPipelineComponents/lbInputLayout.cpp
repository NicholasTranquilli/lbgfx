#include "lbInputLayout.h"

lb::InputLayout::InputLayout(const std::vector<D3D11_INPUT_ELEMENT_DESC>& v_ied, wrl::ComPtr<ID3DBlob>& p_blob)
{
	lb::LB_ENGINE_NAME.GetDevice()->CreateInputLayout(
		v_ied.data(),
		v_ied.size(),
		p_blob->GetBufferPointer(),
		p_blob->GetBufferSize(),
		&this->pInputLayout
	);
}

void lb::InputLayout::Bind()
{
	lb::LB_ENGINE_NAME.GetContext()->IASetInputLayout(pInputLayout.Get());
}
