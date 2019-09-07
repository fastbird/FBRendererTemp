#include "pch.h"
#include "UploadBuffer.h"
#include "Util.h"
#include "RendererD3D12.h"
#include "../../FBCommon/Utility.h"
using namespace fb;

bool UploadBuffer::Initialize(UINT elementSize, UINT align, UINT count)
{
	ElementSizeBeforeAligned = elementSize;
	if (align != 0 || align != 1) {
		elementSize = fb::CalcAligned(elementSize, align);
	}
	ElementSize = elementSize;
	Count = count;
	auto device = gRendererD3D12->GetDevice();
	try {
		ThrowIfFailed(device->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(ElementSize * Count),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&Resource)));

		ThrowIfFailed(Resource->Map(0, nullptr, reinterpret_cast<void**>(&MappedData)));
	}
	catch (const DxException& ex) {
		OutputDebugString(ex.ToString().c_str());
		return false;
	}

	return true;
}

UploadBuffer::~UploadBuffer()
{
	if (Resource != nullptr)
		Resource->Unmap(0, nullptr);

	MappedData = nullptr;
}

void UploadBuffer::CopyData(UINT elementIndex, void* elementData)
{
	memcpy(&MappedData[elementIndex * ElementSize], elementData, ElementSizeBeforeAligned);
}