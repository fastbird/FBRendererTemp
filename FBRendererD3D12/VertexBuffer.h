#pragma once
#include "GPUBuffer.h"
#include "../IRenderer.h"
namespace fb
{
	class VertexBuffer : public GPUBuffer, public IVertexBuffer
	{
		UINT Stride = 0;
		UINT NumVertices = 0;
	public:
		virtual bool Initialize(const void* vertexData, UINT size, UINT stride, bool keepData) override;
		virtual UINT GetSize() const override { return Size; }
		virtual UINT GetStride() const override { return Stride; }
		virtual UINT GetNumVertices() const override { return NumVertices; }
		

		D3D12_VERTEX_BUFFER_VIEW VertexBufferView() const
		{
			return D3D12_VERTEX_BUFFER_VIEW{ Resource->GetGPUVirtualAddress(), Size, Stride };
		}

		virtual void Bind(int startSlot) override;
		virtual void FromUploadBuffer(IUploadBufferIPtr iUploadBuffer) override;
		virtual void* GetCPUAddress() override;
		
	};
}