#pragma once
#include "../FBCommon/IRefCounted.h"
#include "Types.h"

namespace fb
{
	FBDeclareIntrusivePointer(IUploadBuffer);

	FBDeclareIntrusivePointer(IVertexBuffer);
	class IVertexBuffer : public IRefCounted
	{
	public:
		virtual ~IVertexBuffer() {}
		virtual bool Initialize(const void* vertexData, UINT size, UINT stride, bool keepData) = 0;
		virtual UINT GetSize() const = 0;
		virtual UINT GetStride() const = 0;
		virtual UINT GetNumVertices() const = 0;
		virtual void Bind(int startSlot) = 0;
		virtual void FromUploadBuffer(IUploadBufferIPtr uploadBuffer) = 0;
		virtual void* GetCPUAddress() = 0;
	};
}