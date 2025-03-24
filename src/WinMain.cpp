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

	lb::Box vBox;

	while (lb::engine.IsRunning())
	{
		lb::engine.StartFrame();

		static float rx = 0, ry = 0, rz = 0;
		auto vCurrentTick = std::chrono::high_resolution_clock::now().time_since_epoch();
		long long vTimeStamp = std::chrono::duration_cast<std::chrono::milliseconds>(vCurrentTick - vStartTick.time_since_epoch()).count();
		rx = sin(vTimeStamp * (lb::pi_f / 5000.f));
		ry = 2 * sin(vTimeStamp * (lb::pi_f / 5000.f));
		rz = cos(vTimeStamp * (lb::pi_f / 5000.f));

		if (vTimeStamp > 5000 * 2)
			vStartTick = std::chrono::high_resolution_clock::now();
		
		vBox.SetTransformMatrix(lb::MXMatrixRotateZ(rz) * lb::MXMatrixRotateY(ry) * lb::MXMatrixRotateX(rx));
		lb::sColor4f vColor = { (rx + 1) / 2, (rx + 1) / 2, (rz + 1) / 2, 1.f };
		vBox.SetColor(vColor);

		lb::engine.EndFrame();
	}

	lb::engine.CleanUp();

	return 0;
}
