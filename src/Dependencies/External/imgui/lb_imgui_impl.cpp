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

void lb::imgui_impl::ImGui_StartFrame()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void lb::imgui_impl::ImGui_EndFrame()
{
	ImGui::End();
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}
