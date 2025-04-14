// EXAMPLE PROGRAM

// (optional) name change for static global vars macros
#define LB_WINDOW_NAME window
#define LB_ENGINE_NAME engine
#define LB_LOG_NAME logger

// Required include
#include "../lbgfx.h"

// Imgui implementation
#include "../Dependencies/External/imgui/lb_imgui_impl.h"

#include <chrono>

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

BOOL WINAPI ImGui_impl_example(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow
)
{
	auto vStartTick = std::chrono::high_resolution_clock::now();

	lb::logger.SetConsoleOutput(true);

	try
	{
		lb::window.SetWinproc(lb::imgui_impl::ImWndProc);
		lb::engine.Initialize();
		lb::engine.SetBackgroundColor({ 0.1, 0.1, 0.1, 1 });

		lb::engine.GetCamera().SetCameraPos({ 0, 0, -2 });

		lb::imgui_impl::InitializeImGui();

		lb::Box vBox;

		lb::engine.Run([&vStartTick, &vBox]()
			{
				lb::imgui_impl::ImGui_StartFrame("lb ImGui integration");

				static float s = 0.5f;
				static float rx = 0, ry = 0, rz = 0;
				static float x = 0, y = 0, z = 0;
				static bool r = false;

				ImGui::Text("lb Demo using ImGui UI controller");

				ImGui::SliderFloat("x", &x, -1.0f, 1.0f);
				ImGui::SliderFloat("y", &y, -1.0f, 1.0f);
				ImGui::SliderFloat("z", &z, -1.0f, 1.0f);

				if (!r)
				{
					ImGui::SliderFloat("rx", &rx, -10.0f, 10.0f);
					ImGui::SliderFloat("ry", &ry, -10.0f, 10.0f);
					ImGui::SliderFloat("rz", &rz, -10.0f, 10.0f);
				}
				else
				{
					ImGui::BeginDisabled();
					ImGui::SliderFloat("rx", &rx, -10.0f, 10.0f);
					ImGui::SliderFloat("ry", &ry, -10.0f, 10.0f);
					ImGui::SliderFloat("rz", &rz, -10.0f, 10.0f);
					ImGui::EndDisabled();
				}

				ImGui::Checkbox("Auto Rotate?", &r);

				if (r)
				{
					static int vMultiplyer = 1;
					static int vSpeed = 2;
					ImGui::SliderInt("Scalar", &vMultiplyer, -10, 10);
					ImGui::SliderInt("Speed", &vSpeed, 1, 10);

					auto vCurrentTick = std::chrono::high_resolution_clock::now().time_since_epoch();
					long long vTimeStamp = std::chrono::duration_cast<std::chrono::milliseconds>(vCurrentTick - vStartTick.time_since_epoch()).count();
					rx = vMultiplyer * sin(vTimeStamp * (3.1415926535f / ((1.f / vSpeed) * 5000.f)));
					ry = vMultiplyer * 2 * sin(vTimeStamp * (3.1415926535f / ((1.f / vSpeed) * 5000.f)));
					rz = vMultiplyer * cos(vTimeStamp * (3.1415926535f / ((1.f / vSpeed) * 5000.f)));

					if (vTimeStamp > 5000 * 2)
					{
						vStartTick = std::chrono::high_resolution_clock::now();
					}
				}

				vBox.SetTransformMatrix(lb::MXMatrixRotateZ(rz) * lb::MXMatrixRotateY(ry) * lb::MXMatrixRotateX(rx) * lb::MXMatrixTranslate(x, y, z));

				lb::imgui_impl::ImGui_EndFrame();
			});
	}
	LB_CATCH_ALL;

	lb::LB_ENGINE_NAME.CleanUp();
	return 0;
}
