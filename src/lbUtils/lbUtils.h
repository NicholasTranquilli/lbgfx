#pragma once

#include <array>
#include "../Dependencies/Matrix/Matrix.h"
#include "../Dependencies/Matrix/Matrix_gfx_utils.h"

namespace lb
{
	constexpr const double pi_d = 3.141592653589793;
	constexpr const float pi_f = 3.1415926f;

	struct sColor4f
	{
		float r, g, b, a;

		sColor4f();
		sColor4f(float v_r, float v_g, float v_b, float v_a);

		std::array<float, 4> ToArray();
	};
}