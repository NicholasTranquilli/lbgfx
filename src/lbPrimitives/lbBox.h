#pragma once

#include "../lbDrawable/lbPipeline/lbPipelineObject.h"
#include "lbDefaultPrimitive/lbDefaultPrimitive.h"

namespace lb
{
	class Box : public IDefaultPrimitive
	{
	protected:
		void Build() override;

	public:
		Box();
	};
}

