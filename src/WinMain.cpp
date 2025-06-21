// Simple example

// (optional) name change for static global vars macros
#define LB_WINDOW_NAME window
#define LB_ENGINE_NAME engine
#define LB_LOG_NAME logger

// Required include
#include "lbgfx.h"
#include <chrono>

BOOL WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow
)
{
	auto vStartTick = std::chrono::high_resolution_clock::now();

	lb::logger.SetConsoleOutput(true);
	lb::engine.Initialize();
	lb::engine.SetBackgroundColor({ 0.1f, 0.1f, 0.1f, 1.f });

	lb::engine.GetCamera().SetCameraPos({ 0, 0, -2 });

	lb::String2D v("D:/Resources/test3-lg.otf", "Hello\nWorld!");

	v.SetColor({ 1, 0, 0, 1 });

	while (lb::engine.IsRunning())
	{
		lb::engine.StartFrame();

		auto vCurrentTick = std::chrono::high_resolution_clock::now().time_since_epoch();
		long long vTimeStamp = std::chrono::duration_cast<std::chrono::milliseconds>(vCurrentTick - vStartTick.time_since_epoch()).count();
		//v.SetTransformMatrix(lb::MXMatrixRotateY((float)vTimeStamp / 1000.f));

		lb::engine.EndFrame();
	}

	lb::engine.CleanUp();

	return 0;
}