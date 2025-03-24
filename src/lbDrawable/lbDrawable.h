#pragma once

#include "../Dependencies/Matrix/Matrix.h"
#include "../lbError/lbException.h"

namespace lb 
{
	class IDrawable : public lb::Exception
	{
	protected:

	public:
		virtual void Draw() = 0;
		virtual void Update() = 0;
	};
}