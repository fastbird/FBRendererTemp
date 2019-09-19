#include "pch.h"
#include "GPUBuffer.h"
#include "RendererD3D12.h"
#include "Util.h"
using namespace fb;

bool GPUBuffer::Initialize(const void* data, UINT size, bool keepData)
{
	Size = size;
	if (data && size > 0)
	{
		Resource = gRendererD3D12->CreateBufferInDefaultHeap(data, size);
	}

	if (keepData && data) {
		ThrowIfFailed(D3DCreateBlob(size, &CPUData));
		CopyMemory(CPUData->GetBufferPointer(), data, size);
	}
	return Resource != nullptr || !data;
}