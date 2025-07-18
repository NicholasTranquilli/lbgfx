#pragma once

#ifdef LB_USE_IMGUI

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
		void ImGui_StartFrame(const char* v_guiName);
		void ImGui_EndFrame();
		LRESULT __stdcall ImWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	}
}

#endif