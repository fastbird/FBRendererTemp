#pragma once
#include "GPUBuffer.h"
#include "../IRenderer.h"
namespace fb
{
	class IndexBuffer : public GPUBuffer, public IIndexBuffer
	{
		DXGI_FORMAT Format;

	public:
		virtual bool Initialize(const void* indexData, UINT size, EDataFormat format, bool keepData) override;

		D3D12_INDEX_BUFFER_VIEW IndexBufferView() const
		{
			return D3D12_INDEX_BUFFER_VIEW{ Resource->GetGPUVirtualAddress(), Size, Format };
		}
	};
}