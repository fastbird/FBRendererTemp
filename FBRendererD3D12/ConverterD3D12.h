#pragma once
#include "../IRenderer.h"
#include "RootSignature.h"
namespace fb {
	inline DXGI_FORMAT Convert(EDataFormat format) {
		return (DXGI_FORMAT)format;
	}

	inline EDataFormat Convert(DXGI_FORMAT format) {
		return (EDataFormat)format;
	}

	inline D3D12_SHADER_BYTECODE Convert(const FShaderByteCode& s) {
		return D3D12_SHADER_BYTECODE{ s.pShaderBytecode, s.BytecodeLength };
	}

	inline D3D12_STREAM_OUTPUT_DESC Convert(const FStreamOutputDesc& s) {
		return D3D12_STREAM_OUTPUT_DESC{
			(const D3D12_SO_DECLARATION_ENTRY*)s.pSODeclaration, s.NumEntries,
			s.pBufferStrides, s.NumStrides,
			s.RasterizedStream };
	}

	inline D3D12_BLEND Convert(const EBlend s) {
		return (D3D12_BLEND)s;
	}

	inline D3D12_BLEND_OP Convert(const EBlendOp s) {
		return (D3D12_BLEND_OP)s;
	}

	inline D3D12_LOGIC_OP Convert(const ELogicOp s) {
		return D3D12_LOGIC_OP(s);
	}

	inline D3D12_RENDER_TARGET_BLEND_DESC Convert(const FRenderTargetBlendDesc& s) {
		return D3D12_RENDER_TARGET_BLEND_DESC{ s.BlendEnable, s.LogicOpEnable,
			Convert(s.SrcBlend), Convert(s.DestBlend), Convert(s.BlendOp),
		Convert(s.SrcBlendAlpha), Convert(s.DestBlendAlpha), Convert(s.BlendOpAlpha),
		Convert(s.LogicOp), s.RenderTargetWriteMask };
	}

	inline D3D12_BLEND_DESC Convert(const FBlendDesc& s) {
		D3D12_BLEND_DESC desc{ s.AlphaToCoverageEnable, s.IndependentBlendEnable };
		for (int i = 0; i < FB_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i) {
			desc.RenderTarget[i] = Convert(s.RenderTarget[i]);
		}
		return desc;
	}

	inline D3D12_FILL_MODE Convert(EFillMode s) {
		return (D3D12_FILL_MODE)s;
	}

	inline D3D12_CULL_MODE Convert(ECullMode s) {
		return (D3D12_CULL_MODE)s;
	}
	inline D3D12_CONSERVATIVE_RASTERIZATION_MODE Convert(EConservativeRasterizationMode s) {
		return D3D12_CONSERVATIVE_RASTERIZATION_MODE(s);
	}

	inline D3D12_RASTERIZER_DESC Convert(const FRasterizerDesc& s) {
		return D3D12_RASTERIZER_DESC{
			Convert(s.FillMode),
			Convert(s.CullMode),
			s.FrontCounterClockwise,
			s.DepthBias,
			s.DepthBiasClamp,
			s.SlopeScaledDepthBias,
			s.DepthClipEnable,
			s.MultisampleEnable,
			s.AntialiasedLineEnable,
			s.ForcedSampleCount,
			Convert(s.ConservativeRaster)
		};
	}

	inline D3D12_DEPTH_WRITE_MASK Convert(EDepthWriteMask s) {
		return D3D12_DEPTH_WRITE_MASK(s);
	}

	inline D3D12_COMPARISON_FUNC Convert(EComparisonFunc s) {
		return D3D12_COMPARISON_FUNC(s);
	}

	inline D3D12_STENCIL_OP Convert(EStencilOp s) {
		return D3D12_STENCIL_OP(s);
	}

	inline D3D12_DEPTH_STENCILOP_DESC Convert(const FDepthStencilOpDesc& s) {
		return D3D12_DEPTH_STENCILOP_DESC{
			Convert(s.StencilFailOp),
			Convert(s.StencilDepthFailOp),
			Convert(s.StencilPassOp),
			Convert(s.StencilFunc),
		};
	}

	inline D3D12_DEPTH_STENCIL_DESC Convert(const FDepthStencilDesc& s) {
		return D3D12_DEPTH_STENCIL_DESC{
			s.DepthEnable,
			Convert(s.DepthWriteMask),
			Convert(s.DepthFunc),
			s.StencilEnable,
			s.StencilReadMask,
			s.StencilWriteMask,
			Convert(s.FrontFace),
			Convert(s.BackFace)
		};
	}

	inline D3D12_INPUT_LAYOUT_DESC Convert(const FInputLayoutDesc& s)
	{
		assert(sizeof(D3D12_INPUT_ELEMENT_DESC) == sizeof(FInputElementDesc));
		return D3D12_INPUT_LAYOUT_DESC{
			(const D3D12_INPUT_ELEMENT_DESC*)s.pInputElementDescs,
			s.NumElements
		};
	}

	inline D3D12_INDEX_BUFFER_STRIP_CUT_VALUE Convert(EIndexBufferStripCutValue s) {
		return D3D12_INDEX_BUFFER_STRIP_CUT_VALUE(s);
	}

	inline D3D12_PRIMITIVE_TOPOLOGY_TYPE Convert(EPrimitiveTopologyType s) {
		return D3D12_PRIMITIVE_TOPOLOGY_TYPE(s);
	}

	inline 	DXGI_SAMPLE_DESC Convert(const FSampleDesc& s) {
		return DXGI_SAMPLE_DESC{ s.Count, s.Quality };
	}

	inline D3D12_CACHED_PIPELINE_STATE ConvertToCachedPipelineState(const ByteArray& s) {
		return D3D12_CACHED_PIPELINE_STATE{ s.data(), s.size() };
	}

	inline D3D12_PIPELINE_STATE_FLAGS Convert(EPipelineStateFlags flag) {
		return D3D12_PIPELINE_STATE_FLAGS(flag);
	}

	inline 	D3D12_GRAPHICS_PIPELINE_STATE_DESC Convert(const FPSODesc& s)
	{
		D3D12_GRAPHICS_PIPELINE_STATE_DESC desc = {
			((RootSignature*)s.pRootSignature.get())->RootSignature.Get(),
			Convert(s.VS),
			Convert(s.PS),
			Convert(s.DS),
			Convert(s.HS),
			Convert(s.GS),
			Convert(s.StreamOutput),
			Convert(s.BlendState),
			s.SampleMask,
			Convert(s.RasterizerState),
			Convert(s.DepthStencilState),
			Convert(s.InputLayout),
			Convert(s.IBStripCutValue),
			Convert(s.PrimitiveTopologyType),
			s.NumRenderTargets,
			{}, // rtv formats
			Convert(s.DSVFormat),
			Convert(s.SampleDesc),
			s.NodeMask,
			ConvertToCachedPipelineState(s.CachedPSO),
			Convert(s.Flags)
		};
		for (int i = 0; i < FB_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i) {
			desc.RTVFormats[i] = Convert(s.RTVFormats[i]);
		}
		return desc;
	}

	inline D3D_PRIMITIVE_TOPOLOGY Convert(EPrimitiveTopology s) {
		return D3D_PRIMITIVE_TOPOLOGY(s);
	}
}