#pragma once
#include "../IRenderer.h"
namespace fb
{
	class GPUBuffer
	{
	protected:
		Microsoft::WRL::ComPtr<ID3D12Resource> Resource;
		Microsoft::WRL::ComPtr<ID3DBlob> CPUData;
		UINT Size = 0;

	public:
		bool Initialize(const void* data, UINT size, bool keepData);
	};
}
