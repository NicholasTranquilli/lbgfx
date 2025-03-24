#include "lbUtils.h"
#include "../lbError/lbException.h"

/////////////
// Structs //
/////////////

lb::sColor4f::sColor4f()
{
	this->r = 0.f;
	this->g = 0.f;
	this->b = 0.f;
	this->a = 0.f;
}

lb::sColor4f::sColor4f(float v_r, float v_g, float v_b, float v_a)
{
	this->r = v_r;
	this->g = v_g;
	this->b = v_b;
	this->a = v_a;
}

std::array<float, 4> lb::sColor4f::ToArray()
{
	return std::array<float, 4>({ this->r, this->g, this->b, this->a });
}
