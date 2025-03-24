#pragma once

#include "../lbPipelineComponent.h"

namespace lb
{
	class CustomBindable : public lb::IPipelineComponent
	{
	public:
		typedef void (*CustomBindingFn)(void);
		
		CustomBindable(CustomBindingFn p_bindFn);
		void Bind() override;
	
	private:
		CustomBindingFn pBindFn;

	};
}