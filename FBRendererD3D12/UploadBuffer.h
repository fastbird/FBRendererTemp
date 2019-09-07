#pragma once
#include "../IRenderer.h"
namespace fb
{
	class UploadBuffer : public IUploadBuffer
	{
		friend class RendererD3D12;
		Microsoft::WRL::ComPtr<ID3D12Resource> Resource;
		BYTE* MappedData = nullptr;
		UINT ElementSize;
		UINT ElementSizeBeforeAligned;
		UINT Count;

	public:
		UploadBuffer() {}
		UploadBuffer(const UploadBuffer& rhs) = delete;
		UploadBuffer& operator=(const UploadBuffer& rhs) = delete;
		~UploadBuffer();
		virtual bool Initialize(UINT elementSize, UINT align, UINT count) override;
		virtual void CopyData(UINT elementIndex, void* elementData) override;

	};
}
