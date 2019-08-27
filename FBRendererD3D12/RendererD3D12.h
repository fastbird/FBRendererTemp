#pragma once
#include "../IRenderer.h"
namespace fb
{
	class IRenderer;

	class RendererD3D12 : public IRenderer
	{
		Microsoft::WRL::ComPtr<IDXGIFactory4> DXGIFactory;
		Microsoft::WRL::ComPtr<ID3D12Device> Device;
		Microsoft::WRL::ComPtr<ID3D12Fence> Fence;
		Microsoft::WRL::ComPtr<ID3D12CommandQueue> CommandQueue;
		Microsoft::WRL::ComPtr<ID3D12CommandAllocator> DirectCmdAllocator;
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> CommandList;
		Microsoft::WRL::ComPtr<IDXGISwapChain> SwapChain;
		static const int SwapChainBufferCount = 2;
		Microsoft::WRL::ComPtr<ID3D12Resource> SwapChainBuffer[SwapChainBufferCount];
		Microsoft::WRL::ComPtr<ID3D12Resource> DepthStencilBuffer;
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> RtvHeap;
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> DsvHeap;
		// RTV
		// DSV
		// SRV/CBV/UAV
		HWND WindowHandle = 0;
		UINT RtvDescriptorSize = 0;
		UINT DsvDescriptorSize = 0;
		UINT CbvSrvUavDescriptorSize = 0;

		DXGI_FORMAT BackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
		DXGI_FORMAT DepthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
		bool      Msaa4xState = false;    // 4X MSAA enabled
		UINT      NumMsaa4xQualityLevels = 0;      // quality level of 4X MSAA
		
		UINT64 CurrentFence = 0;
		int CurrBackBuffer = 0;

		D3D12_VIEWPORT ScreenViewport;
		D3D12_RECT ScissorRect;


	public:

		virtual bool Initialize(void* windowHandle) override;
		virtual bool Finalize() override;
		virtual void OnResized() override;
		virtual void Draw(float dt) override;

		// Owning Functions
		Microsoft::WRL::ComPtr<ID3D12Resource> CreateDefaultBuffer(
			const void* initData,
			UINT64 byteSize);


	private:

		void LogAdapters();
		void LogAdapterOutputs(IDXGIAdapter* adapter);
		void LogOutputDisplayModes(IDXGIOutput* output, DXGI_FORMAT format);

		void CreateCommandObjects();
		void CreateSwapChain();
		void CreateRtvAndDsvDescriptorHeaps();

		UINT GetClientWidth();
		UINT GetClientHeight();

		void FlushCommandQueue();

		ID3D12Resource* CurrentBackBuffer()const;
		D3D12_CPU_DESCRIPTOR_HANDLE CurrentBackBufferView()const;
		D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView()const;
	};
	extern RendererD3D12* gRendererD3D12;
}

extern "C"
{
	FBRendererD3D12_DLL fb::IRenderer* CreateRendererD3D12();
	FBRendererD3D12_DLL void DeleteRendererD3D12(fb::IRenderer* renderer);
}