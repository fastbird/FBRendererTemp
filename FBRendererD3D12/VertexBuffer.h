#pragma once
#include "GPUBuffer.h"
#include "../IRenderer.h"
namespace fb
{
	class VertexBuffer : public GPUBuffer, public IVertexBuffer
	{
		UINT Stride = 0;
	public:
		virtual bool Initialize(const void* vertexData, UINT size, UINT stride, bool keepData) override;

		D3D12_VERTEX_BUFFER_VIEW VertexBufferView() const
		{
			return D3D12_VERTEX_BUFFER_VIEW{ Resource->GetGPUVirtualAddress(), Size, Stride };
		}
	};
}