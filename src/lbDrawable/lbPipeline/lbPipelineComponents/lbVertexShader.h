#pragma once

#include "../lbPipelineComponent.h"

namespace lb
{
	class VertexShader : public lb::IPipelineComponent
	{
	private:
		wrl::ComPtr<ID3D11VertexShader> pVertexShader;
		wrl::ComPtr<ID3DBlob> pBlob;

	public:
		VertexShader(std::wstring v_fileName);
		void Bind() override;
		wrl::ComPtr<ID3DBlob> GetBlob();
	};
}