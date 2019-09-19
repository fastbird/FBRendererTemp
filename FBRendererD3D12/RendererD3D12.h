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
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> DefaultCbvHeap;
		Microsoft::WRL::ComPtr<ID3D12RootSignature> RootSignature;
		std::unordered_map<PSOID, Microsoft::WRL::ComPtr<ID3D12PipelineState>> PSOs;
		PSOID NextPSOId = 1;

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
		UINT      Msaa4xQuality = 0;      // quality level of 4X MSAA
		
		UINT64 CurrentFence = 0;
		int CurrBackBuffer = 0;

		D3D12_VIEWPORT ScreenViewport;
		D3D12_RECT ScissorRect;

		DrawCallbackFunc DrawCallback = nullptr;

	public:

		virtual bool Initialize(void* windowHandle) override;
		virtual void Finalize() override;
		virtual void OnResized() override;
		virtual void RegisterDrawCallback(DrawCallbackFunc func) override;
		virtual void Draw(float dt) override;
		virtual IVertexBuffer* CreateVertexBuffer(const void* vertexData, UINT size, UINT stride, bool keepData) override;
		virtual IIndexBuffer* CreateIndexBuffer(const void* indexData, UINT size, EDataFormat format, bool keepData) override;
		virtual void CreateCBVHeap(ECBVHeapType type) override;
		virtual IUploadBuffer* CreateUploadBuffer(UINT elementSize, UINT count, bool constantBuffer, ECBVHeapType heapType) override;
		virtual PSOID CreateGraphicsPipelineState(const FPSODesc& psoDesc) override;
		virtual IShader* CompileShader(
			const char* filepath, FShaderMacro* macros, int numMacros, EShaderType shaderType, const char* entryFunctionName) override;
		virtual EDataFormat GetBackBufferFormat() const override;
		virtual EDataFormat GetDepthStencilFormat() const override;
		virtual int GetSampleCount() const override;
		virtual int GetMsaaQuality() const override;
		virtual int GetBackbufferWidth() const override;
		virtual int GetBackbufferHeight() const override;
		virtual void BindPSO(PSOID id) override;
		ID3D12GraphicsCommandList* GetGraphicsCommandList() const { return CommandList.Get(); }

		virtual void TempResetCommandList() override;
		virtual void TempCloseCommandList(bool runAndFlush) override;
		virtual void TempBindDescriptorHeap(ECBVHeapType type) override;
		virtual void TempCreateRootSignatureForSimpleBox() override;
		virtual fb::RootSignature TempGetRootSignatureForSimpleBox() override;
		virtual void TempBindRootSignature(fb::RootSignature rootSig) override;
		virtual void TempBindVertexBuffer(const IVertexBufferIPtr& vb) override;
		virtual void TempBindIndexBuffer(const IIndexBufferIPtr& ib) override;
		virtual void TempSetPrimitiveTopology(const fb::EPrimitiveTopology topology) override;
		virtual void TempBindRootDescriptorTable(UINT slot, ECBVHeapType type) override;
		virtual void TempDrawIndexedInstanced(UINT indexCount) override;


		// Owning Functions
		Microsoft::WRL::ComPtr<ID3D12Resource> CreateBufferInDefaultHeap(
			const void* initData,
			UINT64 byteSize);
		
		ID3D12Device* GetDevice() const { return Device.Get(); }
		
		// Add Public Func;

	private:

		void LogAdapters();
		void LogAdapterOutputs(IDXGIAdapter* adapter);
		void LogOutputDisplayModes(IDXGIOutput* output, DXGI_FORMAT format);

		void CreateCommandObjects();
		void CreateSwapChain();
		void CreateRtvAndDsvDescriptorHeaps();

		UINT GetClientWidth() const;
		UINT GetClientHeight() const;

		void FlushCommandQueue();

		ID3D12Resource* CurrentBackBuffer()const;
		D3D12_CPU_DESCRIPTOR_HANDLE CurrentBackBufferView()const;
		D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView()const;

		void BuildDescriptorHeaps();

		// Add Private Func
	};
	extern RendererD3D12* gRendererD3D12;
}

extern "C"
{
	FBRendererD3D12_DLL fb::IRenderer* CreateRendererD3D12();
}