#include "lbViewport.h"

lb::Viewport::Viewport()
{
	vViewPort.Width = lb::LB_WINDOW_NAME.GetWindowWidth();
	vViewPort.Height = lb::LB_WINDOW_NAME.GetWindowHeight();
	vViewPort.MinDepth = 0;
	vViewPort.MaxDepth = 1;
	vViewPort.TopLeftX = 0;
	vViewPort.TopLeftY = 0;
}

void lb::Viewport::Bind()
{
	lb::LB_ENGINE_NAME.GetContext()->RSSetViewports(1u, &this->vViewPort);
}
