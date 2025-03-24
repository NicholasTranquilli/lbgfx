#pragma once

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include <Windows.h>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace lb
{
	namespace imgui_impl
	{
		void InitializeImGui();
		void ImGui_StartFrame();
		void ImGui_EndFrame();
	}
}