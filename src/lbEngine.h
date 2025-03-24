#pragma once

#include <mutex>
#include <memory>
#include <exception>
#include <type_traits>
#include <functional>

#include "DxIncludes.h"
#include "lbWindow/lbWindow.h"
#include "lbError/lbException.h"
#include "lbDrawable/lbDrawable.h"
#include "lbUtils/lbUtils.h"
#include "lbCamera/lbCamera.h"

#ifndef LB_ENGINE_NAME
#define LB_ENGINE_NAME engine
#endif

namespace lb
{
	class Engine;

	using LBFNProcessMsgCallback = std::function<void(int)>;
	using LBFNRunFrameCallback = std::function<void(void)>;

	class Engine : public Exception
	{
	private:
		wrl::ComPtr<ID3D11Device> pDevice;
		wrl::ComPtr<IDXGISwapChain> pSwapChain;
		wrl::ComPtr<ID3D11DeviceContext> pContext;
		wrl::ComPtr<ID3D11RenderTargetView> pTarget;
		wrl::ComPtr<ID3D11Resource> pBackBuffer;
		wrl::ComPtr<ID3D11DepthStencilView> pDepthStencilView;

		wrl::ComPtr<ID3D11ShaderResourceView> pDepthSRV;
		wrl::ComPtr<ID3D11Texture2D> pDepthStencilTexture;

		HWND vOutputHwnd;
		sColor4f vColor;
		bool vIsRuning;
		bool vHasConsole;

		float vNearZ;
		float vFarZ;

		LBFNProcessMsgCallback fnProcessMessageCallback;

		std::vector<IDrawable*> vDrawableObjects;

		Camera vCam;

		Engine() = default;

	public:
		Engine(Engine&) = delete;
		Engine(const Engine&) = delete;

		~Engine();

		inline void SetDefaults();

		inline static Engine& GetInstance();

		inline void Initialize();

		inline void StartFrame();
		inline void EndFrame();
		inline void Run(LBFNRunFrameCallback fn_RunFrameCallback = nullptr);
		inline void CleanUp();

		inline bool IsRunning();

		inline wrl::ComPtr<ID3D11Device>& GetDevice();
		inline wrl::ComPtr<IDXGISwapChain>& GetSwapChain();
		inline wrl::ComPtr<ID3D11DeviceContext>& GetContext();
		inline wrl::ComPtr<ID3D11RenderTargetView>& GetTarget();
		inline wrl::ComPtr<ID3D11Resource>& GetBackBuffer();
		inline wrl::ComPtr<ID3D11DepthStencilView>& GetDepthStencilView();
		inline wrl::ComPtr<ID3D11ShaderResourceView>& GetDepthSRV();
		inline wrl::ComPtr<ID3D11Texture2D>& GetDepthStencilTexture();

		inline void SetProcessEndCallback(LBFNProcessMsgCallback fn_ProcessMessageCallback);

		inline void SetBackgroundColor(sColor4f v_Color);

		inline void SpawnConsole();

		inline void AddDrawable(IDrawable* p_DrawableObject);
		inline void RemoveDrawable(IDrawable* p_DrawableObject);

		inline Camera& GetCamera();

		inline float GetNearZ();
		inline float GetFarZ();

		inline void SetNearZ(float v_nearZ);
		inline void SetFarZ(float v_farZ);
	};

	// Global Instance var
	static Engine& LB_ENGINE_NAME = Engine::GetInstance();

	inline void Terminate();
}

/**********************/
/* EXTERNAL FUNCTIONS */
/**********************/

void lb::Terminate() 
{
	lb::LB_WINDOW_NAME.~Window();
	lb::LB_ENGINE_NAME.~Engine();
	exit(0);
}

/***************************/
/* PUBLIC MEMBER FUNCTIONS */
/***************************/

/// <summary>
/// Engine deconstructor
/// </summary>
inline lb::Engine::~Engine()
{
	return;
}

/// <summary>
/// Sets default engine values
/// </summary>
inline void lb::Engine::SetDefaults()
{
	LBFNProcessMsgCallback fnProcessMsgDefault = [this](int v_error) -> void {
		this->LogInfo(lb::Exception("Process Message Code: " + std::to_string(v_error)));
	};

	this->fnProcessMessageCallback = fnProcessMsgDefault;
	this->vOutputHwnd = lb::LB_WINDOW_NAME.GetHWND();
	this->vIsRuning = true;
	this->vNearZ = 0.5f;
	this->vFarZ = 1000.0f;
}

/// <summary>
/// GetInstance
/// </summary>
/// <returns>current engine singleton instance</returns>
inline lb::Engine& lb::Engine::GetInstance()
{
	try
	{
		static Engine vObjInstance;
		vObjInstance.SetDefaults();

#ifdef _DEBUG
		vObjInstance.SpawnConsole();
#endif // !DEBUG
		
		return vObjInstance;
	}
	LB_CATCH_ALL;
}

/// <summary>
/// Initialize with set engine values
/// </summary>
inline void lb::Engine::Initialize()
{
	// Init window if not done
	if (this->vOutputHwnd == NULL)
	{
		lb::LB_WINDOW_NAME.Initialize();
		this->vOutputHwnd = lb::LB_WINDOW_NAME.GetHWND();
	}

	// Init D3D
	DXGI_SWAP_CHAIN_DESC vSwapDesc = {};
	vSwapDesc.BufferDesc.Width = 0;
	vSwapDesc.BufferDesc.Height = 0;
	vSwapDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	vSwapDesc.BufferDesc.RefreshRate.Numerator = 0;
	vSwapDesc.BufferDesc.RefreshRate.Denominator = 0;
	vSwapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	vSwapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	vSwapDesc.SampleDesc.Count = 1;
	vSwapDesc.SampleDesc.Quality = 0;
	vSwapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	vSwapDesc.BufferCount = 1;
	vSwapDesc.OutputWindow = vOutputHwnd;
	vSwapDesc.Windowed = TRUE;
	vSwapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	vSwapDesc.Flags = 0;

	D3D_FEATURE_LEVEL vRequestedFeatureLevels[] = {
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0
	};

	D3D_FEATURE_LEVEL vResultFeatureLevel;
	
	LB_CHECK_HR_AND_THROW_EX(D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		D3D11_CREATE_DEVICE_DEBUG,
		vRequestedFeatureLevels,
		std::size(vRequestedFeatureLevels),
		D3D11_SDK_VERSION,
		&vSwapDesc,
		&this->pSwapChain,
		&this->pDevice,
		&vResultFeatureLevel,
		&this->pContext
	), "D3D11CreateDeviceAndSwapChain");

	LB_LOG_VALUE(vResultFeatureLevel);
	
	LB_CHECK_HR_AND_THROW_EX(this->pSwapChain->GetBuffer(0, __uuidof(ID3D11Resource), &this->pBackBuffer), "GetBuffer");

	LB_CHECK_HR_AND_THROW_EX(this->pDevice->CreateRenderTargetView(
		this->pBackBuffer.Get(),
		nullptr,
		&this->pTarget
	), "CreateRenderTargetView");

	D3D11_TEXTURE2D_DESC vDepthStencilDesc = {};
	vDepthStencilDesc.Width = lb::LB_WINDOW_NAME.GetWindowWidth();
	vDepthStencilDesc.Height = lb::LB_WINDOW_NAME.GetWindowHeight();
	vDepthStencilDesc.MipLevels = 1;
	vDepthStencilDesc.ArraySize = 1;
	vDepthStencilDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	vDepthStencilDesc.SampleDesc.Count = 1;
	vDepthStencilDesc.SampleDesc.Quality = 0;
	vDepthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	vDepthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	vDepthStencilDesc.CPUAccessFlags = 0;

	LB_CHECK_HR_AND_THROW_EX(this->pDevice->CreateTexture2D(&vDepthStencilDesc, nullptr, &pDepthStencilTexture), "CreateTexture2D");

	D3D11_DEPTH_STENCIL_VIEW_DESC vDsvDesc = {};
	vDsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	vDsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

	LB_CHECK_HR_AND_THROW_EX(this->pDevice->CreateDepthStencilView(pDepthStencilTexture.Get(), &vDsvDesc, &this->pDepthStencilView), "CreateDepthStencilView");

	D3D11_SHADER_RESOURCE_VIEW_DESC vSrvDesc = {};
	vSrvDesc.Format = DXGI_FORMAT_X24_TYPELESS_G8_UINT;
	vSrvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	vSrvDesc.Texture2D.MostDetailedMip = 0;
	vSrvDesc.Texture2D.MipLevels = 1;

	LB_CHECK_HR_AND_THROW_EX(this->pDevice->CreateShaderResourceView(pDepthStencilTexture.Get(), &vSrvDesc, &this->pDepthSRV), "CreateShaderResourceView");
}

/// <summary>
/// Start the frame
/// </summary>
inline void lb::Engine::StartFrame()
{
	// Process message
	if (const auto vIntResult = lb::LB_WINDOW_NAME.ProcessMessage())
	{
		if (this->fnProcessMessageCallback != nullptr)
			this->fnProcessMessageCallback(vIntResult.value());
		
		this->vIsRuning = false;
	}

	// Clear depth stencil view
	this->pContext->ClearDepthStencilView(this->pDepthStencilView.Get(),
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// Clear view with background color
	this->pContext->ClearRenderTargetView(this->pTarget.Get(), this->vColor.ToArray().data());
}

/// <summary>
/// End the frame
/// </summary>
inline void lb::Engine::EndFrame()
{
	// Update frame
	for (auto& v_elem : this->vDrawableObjects)
	{
		v_elem->Update();
		v_elem->Draw();
	}

	pSwapChain->Present(1u, 0u);
}

/// <summary>
/// Process frame (can be used instead of typical StartFrame EndFrame loop)
/// </summary>
inline void lb::Engine::Run(LBFNRunFrameCallback fn_RunFrameCallback)
{
	while (lb::LB_ENGINE_NAME.IsRunning())
	{
		lb::LB_ENGINE_NAME.StartFrame();

		if (fn_RunFrameCallback)
			fn_RunFrameCallback();

		lb::LB_ENGINE_NAME.EndFrame();
	}

	lb::LB_ENGINE_NAME.CleanUp();
}

/// <summary>
/// Remove dangling pointers and free memory space allocated by Engine
/// </summary>
inline void lb::Engine::CleanUp()
{
	for (lb::IDrawable*& v_elem : this->vDrawableObjects) 
		v_elem = nullptr;
}

/// <summary>
/// End the frame
/// </summary>
/// <returns>running status of engine</returns>
inline bool lb::Engine::IsRunning()
{
	return this->vIsRuning;
}

/// <summary>
/// Get device
/// </summary>
/// <returns>ComPtr to device</returns>
inline wrl::ComPtr<ID3D11Device>& lb::Engine::GetDevice()
{
	return this->pDevice;
}

/// <summary>
/// Get swap chain
/// </summary>
/// <returns>ComPtr to swapchain</returns>
inline wrl::ComPtr<IDXGISwapChain>& lb::Engine::GetSwapChain()
{
	return this->pSwapChain;
}

/// <summary>
/// Get context
/// </summary>
/// <returns>ComPtr to context</returns>
inline wrl::ComPtr<ID3D11DeviceContext>& lb::Engine::GetContext()
{
	return this->pContext;
}

/// <summary>
/// Get render target
/// </summary>
/// <returns>ComPtr to render target</returns>
inline wrl::ComPtr<ID3D11RenderTargetView>& lb::Engine::GetTarget()
{
	return this->pTarget;
}

/// <summary>
/// Get back buffer
/// </summary>
/// <returns>ComPtr to back buffer</returns>
inline wrl::ComPtr<ID3D11Resource>& lb::Engine::GetBackBuffer()
{
	return this->pBackBuffer;
}

/// <summary>
/// Get depth stencil view
/// </summary>
/// <returns>ComPtr to depth stencil view</returns>
inline wrl::ComPtr<ID3D11DepthStencilView>& lb::Engine::GetDepthStencilView()
{
	return this->pDepthStencilView;
}

/// <summary>
/// Get depth shader resource view
/// </summary>
/// <returns>ComPtr to depth shader resource view</returns>
inline wrl::ComPtr<ID3D11ShaderResourceView>& lb::Engine::GetDepthSRV()
{
	return this->pDepthSRV;
}

/// <summary>
/// Get depth shader resource texture
/// </summary>
/// <returns>ComPtr to depth shader resource texture</returns>
inline wrl::ComPtr<ID3D11Texture2D>& lb::Engine::GetDepthStencilTexture()
{
	return this->pDepthStencilTexture;
}

/// <summary>
/// Sets the process callback to be called right before program termination
/// </summary>
inline void lb::Engine::SetProcessEndCallback(LBFNProcessMsgCallback fn_ProcessMessageCallback)
{
	this->fnProcessMessageCallback = fn_ProcessMessageCallback;
}

/// <summary>
/// Set background color for render view
/// </summary>
/// <param name="v_Color">screen color</param>
inline void lb::Engine::SetBackgroundColor(sColor4f v_Color)
{
	this->vColor = v_Color;
}

/// <summary>
/// Wrapper for AllocConsole()
/// </summary>
inline void lb::Engine::SpawnConsole()
{
	if (!this->vHasConsole) {
		AllocConsole();
		freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
		this->vHasConsole = true;
	}
}

/// <summary>
/// Appends drawable to internal vector
/// </summary>
/// <param name="p_DrawableElement">address of drawable to be appeneded</param>
inline void lb::Engine::AddDrawable(IDrawable* p_DrawableObject)
{
	this->vDrawableObjects.push_back(p_DrawableObject);
}

/// <summary>
/// Removes drawable from internal vector by address
/// </summary>
/// <param name="p_DrawableElement">address of drawable to be removed</param>
inline void lb::Engine::RemoveDrawable(IDrawable* p_DrawableObject)
{
	auto vObj = std::find(this->vDrawableObjects.begin(), this->vDrawableObjects.end(), p_DrawableObject);

	if (vObj != this->vDrawableObjects.end()) 
		this->vDrawableObjects.erase(vObj);
}

/// <summary>
/// Instance of Camera being used by engine
/// </summary>
/// <returns>lb::Camera object</returns>
inline lb::Camera& lb::Engine::GetCamera()
{
	return this->vCam;
}

/// <summary>
/// Get min z value before culling
/// </summary>
/// <returns>flaot representing near-z</returns>
inline float lb::Engine::GetNearZ()
{
	return this->vNearZ;
}

/// <summary>
/// Get max z value before culling
/// </summary>
/// <returns>flaot representing far-z</returns>
inline float lb::Engine::GetFarZ()
{
	return this->vFarZ;
}

/// <summary>
/// Set min culling distance for projection matrix
/// </summary>
/// <param name="v_farZ">minimum z-axis value before culling</param>
inline void lb::Engine::SetNearZ(float v_nearZ)
{
	this->vNearZ = v_nearZ;
}

/// <summary>
/// Set max culling distance for projection matrix
/// </summary>
/// <param name="v_farZ">maximum z-axis value before culling</param>
inline void lb::Engine::SetFarZ(float v_farZ)
{
	this->vFarZ = v_farZ;
}
