#pragma once

#include "../lbPipelineComponent.h"

namespace lb
{
	class PixelShader : public lb::IPipelineComponent
	{
	private:
		wrl::ComPtr<ID3D11PixelShader> pPixelShader;
		wrl::ComPtr<ID3DBlob> pBlob;

	public:
		PixelShader(std::wstring v_fileName);
		void Bind() override;
	};
}