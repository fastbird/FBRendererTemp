#pragma once
#include "GPUBuffer.h"
#include "../IRenderer.h"
namespace fb
{
	class IndexBuffer : public GPUBuffer, public IIndexBuffer
	{
		DXGI_FORMAT Format;
		UINT ElementCount;

	public:
		virtual bool Initialize(const void* indexData, UINT size, EDataFormat format, bool keepData) override;
		virtual UINT GetSize() const override { return Size; }
		virtual EDataFormat GetFormat() const override;
		virtual UINT GetElementCount() const override { return ElementCount; }
		D3D12_INDEX_BUFFER_VIEW IndexBufferView() const
		{
			return D3D12_INDEX_BUFFER_VIEW{ Resource->GetGPUVirtualAddress(), Size, Format };
		}
		
	};
}