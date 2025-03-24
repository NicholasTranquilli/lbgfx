#pragma once

#include "../lbPipelineComponent.h"

namespace lb 
{
	class IndexBuffer : public lb::IPipelineComponent
	{
	private:
		std::vector<unsigned short> vIndices;
		wrl::ComPtr<ID3D11Buffer> pIndexBuffer;

	public:
		IndexBuffer(std::vector<unsigned short>& v_indices);
		void Bind() override;
	};
}