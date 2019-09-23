#pragma once
#include "../IRootSignature.h"

namespace fb {
	struct FPSODesc;
	class RootSignature : public IRootSignature
	{
		friend class RendererD3D12;
		friend D3D12_GRAPHICS_PIPELINE_STATE_DESC Convert(const FPSODesc& s);

		Microsoft::WRL::ComPtr<ID3D12RootSignature> RootSignature;

	public:
		virtual void Bind() override;
	};
}
