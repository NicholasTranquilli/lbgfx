#include "lbVertexShader.h"

lb::VertexShader::VertexShader(std::wstring v_fileName)
{
	D3DReadFileToBlob(v_fileName.c_str(), &pBlob);
	lb::LB_ENGINE_NAME.GetDevice()->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader);
}

void lb::VertexShader::Bind()
{
	lb::LB_ENGINE_NAME.GetContext()->VSSetShader(pVertexShader.Get(), nullptr, 0u);
}

wrl::ComPtr<ID3DBlob> lb::VertexShader::GetBlob()
{
	return this->pBlob;
}
