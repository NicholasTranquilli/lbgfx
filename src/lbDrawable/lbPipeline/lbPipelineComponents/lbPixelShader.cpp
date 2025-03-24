#include "lbPixelShader.h"

lb::PixelShader::PixelShader(std::wstring v_fileName)
{
	D3DReadFileToBlob(v_fileName.c_str(), &pBlob);
	lb::LB_ENGINE_NAME.GetDevice()->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader);
}

void lb::PixelShader::Bind()
{
	lb::LB_ENGINE_NAME.GetContext()->PSSetShader(pPixelShader.Get(), nullptr, 0u);
}
