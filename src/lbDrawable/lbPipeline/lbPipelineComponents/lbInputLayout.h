#pragma once

#include "../lbPipelineComponent.h"

namespace lb
{
	class InputLayout : public lb::IPipelineComponent
	{
	private:
		wrl::ComPtr<ID3D11InputLayout> pInputLayout;

	public:
		InputLayout(const std::vector<D3D11_INPUT_ELEMENT_DESC>& v_ied, wrl::ComPtr<ID3DBlob>& p_blob);
		void Bind() override;
	};
}