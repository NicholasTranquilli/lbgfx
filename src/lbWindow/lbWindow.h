#pragma once

#include <Windows.h>
#include <mutex>
#include <optional>

#ifndef LB_WINDOW_NAME
#define LB_WINDOW_NAME win
#endif 

namespace lb
{
	enum
	{
		LB_DEFAULT = -1
	};

	typedef LRESULT(LBFNWndProc)(HWND, UINT, WPARAM, LPARAM);
	
    class Window 
	{
    private:
		HWND vWinHandle;
		HINSTANCE vWinInstance;
        
		std::string vStrWinClass;
        std::string vStrWinName;
		
		DWORD vFlags;
		DWORD vFlagsEx;
		int vPosX, vPosY, vWidth, vHeight;

		LBFNWndProc* fnpWinProc;

        Window() = default;

    public:
        Window(Window&) = delete;
        Window(const Window&) = delete;
        
		inline ~Window();
		
		inline void SetDefaults();

        inline static Window& GetInstance();

		inline static std::optional<int> ProcessMessage();

        inline void Initialize();

        inline HWND& GetHWND();

        inline void SetHWND(HWND v_Handle);

		inline void SetWindowFlag(DWORD v_Flag);

		inline void SetWindowFlagEx(DWORD v_FlagEx);

		inline void SetWindowRect(int v_PosX = LB_DEFAULT, int v_PosY = LB_DEFAULT, int v_Width = LB_DEFAULT, int v_Height = LB_DEFAULT);

		inline int GetWindowWidth();

		inline int GetWindowHeight();

		inline float GetInverseAspectRatio();

		inline void SetWinproc(LBFNWndProc* fnp_WinProc);
    };

	// Global Instance var
    static Window& LB_WINDOW_NAME = Window::GetInstance();
}

/***************************/
/* PUBLIC MEMBER FUNCTIONS */
/***************************/

/// <summary>
/// Window decon
/// </summary>
inline lb::Window::~Window()
{
	UnregisterClass(this->vStrWinClass.c_str(), this->vWinInstance);
	DestroyWindow(this->vWinHandle);
}

/// <summary>
/// ProcessMessage
/// </summary>
/// <returns>
/// optional int
/// </returns>
inline std::optional<int> lb::Window::ProcessMessage()
{
	MSG msg;
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT) return (int)msg.wParam;

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return {};
}

/// <summary>
/// Private function sets Window defaults
/// </summary>
inline void lb::Window::SetDefaults()
{
	this->vPosX = 0;
	this->vPosY = 0;
	this->vWidth = 1000;
	this->vHeight = 800;

	this->vStrWinClass = "LB_GAME_ENGINE_APP";
	this->vStrWinName = "My lbe app!";

	this->vFlags = WS_SYSMENU | WS_VISIBLE;
	this->vFlagsEx = NULL;

	this->vWinHandle = 0;

	LBFNWndProc* fnpDefWindowProc = [](HWND v_hWnd, UINT v_msg, WPARAM v_wParam, LPARAM lParam) -> LRESULT {
			switch (v_msg)
			{
			case WM_CLOSE:
				PostQuitMessage(0);
				return 0;
				break;
			default:
				break;
			}

			return DefWindowProc(v_hWnd, v_msg, v_wParam, lParam);
	};

	this->SetWinproc(fnpDefWindowProc);
}

/// <summary>
/// GetInstance
/// </summary>
/// <returns>
/// static Window* instance
/// </returns>
inline lb::Window& lb::Window::GetInstance()
{
	static lb::Window vObjInstance;
	vObjInstance.SetDefaults();
	return vObjInstance;
}

/// <summary>
/// Initialize
/// </summary>
/// <returns>
/// bool success or failure
/// </returns>
inline void lb::Window::Initialize()
{
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = fnpWinProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = vWinInstance;
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = vStrWinClass.c_str();

	RegisterClassEx(&wc);

	this->vWinHandle = CreateWindowExA(
		this->vFlagsEx,
		this->vStrWinClass.c_str(),
		this->vStrWinName.c_str(),
		this->vFlags,
		this->vPosX,
		this->vPosY,
		this->vWidth,
		this->vHeight,
		nullptr,
		nullptr,
		this->vWinInstance,
		nullptr
	);
}

/// <summary>
/// GetHWND
/// </summary>
/// <returns>
/// HWND&
/// </returns>
inline HWND& lb::Window::GetHWND()
{
    return this->vWinHandle;
}

/// <summary>
/// SetHWND
/// </summary>
/// <param name="vHandle">
/// Handle to existing window
/// </param>
inline void lb::Window::SetHWND(HWND v_Handle)
{
    this->vWinHandle = v_Handle;
}

/// <summary>
/// SetWindowFlag
/// </summary>
/// <param name="v_Flag">
/// Set Window flag
/// </param>
inline void lb::Window::SetWindowFlag(DWORD v_Flag)
{
	this->vFlags = v_Flag;
}

/// <summary>
/// SetWindowFlagEx
/// </summary>
/// <param name="v_FlagEx">
/// Set Window extended flags
/// </param>
inline void lb::Window::SetWindowFlagEx(DWORD v_FlagEx)
{
	this->vFlagsEx = v_FlagEx;
}

/// <summary>
/// Set window position and size before initialize
/// </summary>
/// <param name="v_PosX">start pos x</param>
/// <param name="v_PosY">start pos y</param>
/// <param name="v_Width">width</param>
/// <param name="v_Height">height</param>
inline void lb::Window::SetWindowRect(int v_PosX, int v_PosY, int v_Width, int v_Height)
{
	if (v_PosX != LB_DEFAULT) this->vPosX = v_PosX;
	if (v_PosY != LB_DEFAULT) this->vPosY = v_PosY;
	if (v_Width != LB_DEFAULT) this->vWidth = v_Width;
	if (v_Height != LB_DEFAULT) this->vHeight= v_Height;
}

/// <summary>
/// Get window width
/// </summary>
/// <returns>window width as int</returns>
inline int lb::Window::GetWindowWidth()
{
	return this->vWidth;
}

/// <summary>
/// Get window height
/// </summary>
/// <returns>window height as int</returns>
inline int lb::Window::GetWindowHeight()
{
	return this->vHeight;
}

/// <summary>
/// Get height / width
/// </summary>
/// <returns>float aspect ratio of screen</returns>
inline float lb::Window::GetInverseAspectRatio()
{
	return (float)this->vHeight / (float)this->vWidth;
}

/// <summary>
///  Set static window proc for instance
/// </summary>
/// <param name="fnp_WinProc">function pointer to new window proc function</param>
inline void lb::Window::SetWinproc(LBFNWndProc* fnp_WinProc)
{
	this->fnpWinProc = fnp_WinProc;
}
