#pragma once

#include "../lbPipelineComponent.h"

namespace lb
{
	class Viewport : public lb::IPipelineComponent
	{
	private:
		D3D11_VIEWPORT vViewPort;

	public:
		Viewport();
		void Bind() override;
	};
}
