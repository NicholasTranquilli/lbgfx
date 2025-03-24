#pragma once

#include "../../lbEngine.h"

namespace lb
{
	enum class ShaderType
	{
		SHADER_INVALID,
		VS,
		PS,
		SHADER_MAX,
	};

	class IPipelineComponent : protected lb::Exception
	{
	private:

	public:
		IPipelineComponent() = default;

		virtual void Bind() = 0;
	};
}