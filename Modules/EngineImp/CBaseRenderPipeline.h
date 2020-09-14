//***************************************************************************************
// d3dApp.h by Frank Luna (C) 2015 All Rights Reserved.
//***************************************************************************************

#pragma once

#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

#include "../Common/d3dUtil.h"
#include "../Common/GameTimer.h"
#include "../Common/GeometryDefines.h"
#include <DirectXCollision.h>
#include <map>

// Link necessary d3d12 libraries.
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "dxgi.lib")

struct RenderItem;

class IRenderPipeline
{
public:
	virtual bool Initialize() = 0;
	virtual void OnResize() = 0;
	virtual void Update(const GameTimer& gt) = 0;
	virtual void UpdateCamera(const GameTimer& gt) = 0;
	virtual void Draw(const GameTimer& gt) = 0;
    virtual void PushMats(std::vector<RenderItem*>& render_items) = 0;
	virtual bool InitDirect3D() = 0;
	virtual void Debug() = 0;
	virtual void PitchCamera(float rad) = 0;
	virtual void RotateCameraY(float rad) = 0;
	virtual void MoveCamera(float dis) = 0;
	virtual void StrafeCamera(float dis) = 0;
	virtual std::vector<RenderItem*>& GetRenderItems(int layer) = 0;
	virtual DirectX::XMFLOAT3 GetCameraPos() = 0;
	virtual DirectX::BoundingFrustum GetCameraFrustum() = 0;
	virtual DirectX::XMFLOAT3 GetCameraDir() = 0;
	virtual void ClearVisibleRenderItems() = 0;
	virtual void PushVisibleModels(std::map<int,  std::vector<RenderItem*>>& render_items, bool add = false) = 0;
	virtual bool IsCameraDirty() = 0;

};

class CBaseRenderPipeline : public IRenderPipeline
{
protected:

    CBaseRenderPipeline(HINSTANCE hInstance, HWND wnd);
    CBaseRenderPipeline(const CBaseRenderPipeline& rhs) = delete;
    CBaseRenderPipeline& operator=(const CBaseRenderPipeline& rhs) = delete;
    virtual ~CBaseRenderPipeline();

public:

    static CBaseRenderPipeline* GetApp();
    virtual void Debug() override;
	HINSTANCE AppInst()const;
	HWND      MainWnd()const;
	float     AspectRatio()const;

    bool Get4xMsaaState()const;
    void Set4xMsaaState(bool value);
	virtual bool InitDirect3D();
	virtual bool Initialize();
    virtual void PushMats(std::vector<RenderItem*>& render_items) = 0;
	virtual void PitchCamera(float rad) = 0;
	virtual void RotateCameraY(float rad) = 0;
	virtual void MoveCamera(float dis) = 0;
	virtual void StrafeCamera(float dis) = 0;
protected:
    virtual void CreateRtvAndDsvDescriptorHeaps();
	virtual void OnResize(); 
	virtual void Update(const GameTimer& gt)=0;
    virtual void Draw(const GameTimer& gt)=0;
	virtual void UpdateCamera(const GameTimer& gt) = 0;
	virtual std::vector<RenderItem*>& GetRenderItems(int layer) = 0;
	virtual DirectX::XMFLOAT3 GetCameraPos() = 0;
	virtual DirectX::BoundingFrustum GetCameraFrustum() = 0;
	virtual DirectX::XMFLOAT3 GetCameraDir() = 0;
	virtual void ClearVisibleRenderItems() = 0;
	virtual void PushVisibleModels(std::map<int,  std::vector<RenderItem*>>& render_items, bool add = false) = 0;
	virtual bool IsCameraDirty() = 0;

protected:

	void CreateCommandObjects();
    void CreateSwapChain();

	void FlushCommandQueue();

	ID3D12Resource* CurrentBackBuffer()const;
	D3D12_CPU_DESCRIPTOR_HANDLE CurrentBackBufferView()const;
	D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView()const;


    void LogAdapters();
    void LogAdapterOutputs(IDXGIAdapter* adapter);
    void LogOutputDisplayModes(IDXGIOutput* output, DXGI_FORMAT format);

protected:

    static CBaseRenderPipeline* m_engine;

    HINSTANCE mhAppInst = nullptr; // application instance handle
    HWND      mhMainWnd = nullptr; // main window handle
	bool      mAppPaused = false;  // is the application paused?
	bool      mMinimized = false;  // is the application minimized?
	bool      mMaximized = false;  // is the application maximized?
	bool      mResizing = false;   // are the resize bars being dragged?
    bool      mFullscreenState = false;// fullscreen enabled

	// Set true to use 4X MSAA (?.1.8).  The default is false.
    bool      m4xMsaaState = false;    // 4X MSAA enabled
    UINT      m4xMsaaQuality = 0;      // quality level of 4X MSAA

	// Used to keep track of the �delta-time?and game time (?.4).
	GameTimer mTimer;
	
    Microsoft::WRL::ComPtr<IDXGIFactory4> mdxgiFactory;
    Microsoft::WRL::ComPtr<IDXGISwapChain> mSwapChain;
    Microsoft::WRL::ComPtr<ID3D12Device> md3dDevice;

    Microsoft::WRL::ComPtr<ID3D12Fence> mFence;
    UINT64 mCurrentFence = 0;
	
    Microsoft::WRL::ComPtr<ID3D12CommandQueue> mCommandQueue;
    Microsoft::WRL::ComPtr<ID3D12CommandAllocator> mDirectCmdListAlloc;
    Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> mCommandList;

	static const int SwapChainBufferCount = 2;
	int mCurrBackBuffer = 0;
    Microsoft::WRL::ComPtr<ID3D12Resource> mSwapChainBuffer[SwapChainBufferCount];
    Microsoft::WRL::ComPtr<ID3D12Resource> mDepthStencilBuffer;

    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mRtvHeap;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mDsvHeap;

    D3D12_VIEWPORT mScreenViewport; 
    D3D12_RECT mScissorRect;

	UINT mRtvDescriptorSize = 0;
	UINT mDsvDescriptorSize = 0;
	UINT mCbvSrvUavDescriptorSize = 0;

	// Derived class should set these in derived constructor to customize starting values.
	D3D_DRIVER_TYPE md3dDriverType = D3D_DRIVER_TYPE_HARDWARE;
    DXGI_FORMAT mBackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
    DXGI_FORMAT mDepthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	int mClientWidth =1024;
	int mClientHeight = 1024;


};

