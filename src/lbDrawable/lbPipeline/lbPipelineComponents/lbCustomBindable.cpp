#include "lbCustomBindable.h"

lb::CustomBindable::CustomBindable(CustomBindingFn p_bindFn)
	: pBindFn(p_bindFn)
{
}

void lb::CustomBindable::Bind()
{
	this->pBindFn();
}
