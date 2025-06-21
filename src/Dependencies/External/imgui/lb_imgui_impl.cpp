#ifdef LB_USE_IMGUI

#include "lb_imgui_impl.h"
#include "../../../lbEngine.h"

void lb::imgui_impl::InitializeImGui()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(lb::LB_WINDOW_NAME.GetHWND());
	ImGui_ImplDX11_Init(lb::LB_ENGINE_NAME.GetDevice().Get(), lb::LB_ENGINE_NAME.GetContext().Get());
}

void lb::imgui_impl::ImGui_StartFrame(const char* v_guiName)
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGui::Begin(v_guiName);
}

void lb::imgui_impl::ImGui_EndFrame()
{
	ImGui::End();
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

LRESULT __stdcall lb::imgui_impl::ImWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;

	switch (msg)
	{
	case WM_DESTROY:
		::PostQuitMessage(0);
		return 0;
	}

	return ::DefWindowProc(hWnd, msg, wParam, lParam);
}

#endif