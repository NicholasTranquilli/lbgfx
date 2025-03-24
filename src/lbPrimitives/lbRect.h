#pragma once

#include "../lbDrawable/lbPipeline/lbPipelineObject.h"
#include "lbDefaultPrimitive/lbDefaultPrimitive.h"

namespace lb 
{
	class Rect : public lb::IDefaultPrimitive
	{
	protected:
		void Build() override;

	public:
		Rect();
	};
}
