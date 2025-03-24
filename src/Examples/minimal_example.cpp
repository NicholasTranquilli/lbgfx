// Minimal example program
// Render a red rectangle

// (optional) name change for static global vars macros
#define LB_WINDOW_NAME window
#define LB_ENGINE_NAME engine
#define LB_LOG_NAME logger

// Required include
#include "../lbgfx.h"

BOOL WINAPI minimal_example(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow
)
{
	lb::engine.Initialize();
	lb::engine.SetBackgroundColor({ 0.1, 0.1, 0.1, 1 });

	lb::Rect vRect;

	vRect.SetColor({255, 0, 0, 255});

	lb::engine.Run();

	lb::LB_ENGINE_NAME.CleanUp();

	return 0;
}
