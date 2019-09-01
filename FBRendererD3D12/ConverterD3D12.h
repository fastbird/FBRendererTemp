#pragma once
#include "../IRenderer.h"
namespace fb {
	DXGI_FORMAT Convert(EDataFormat format) {
		return (DXGI_FORMAT)format;
	}

	EDataFormat Convert(DXGI_FORMAT format) {
		return (EDataFormat)format;
	}

	D3D12_SHADER_BYTECODE Convert(const FShaderByteCode& s) {
		return D3D12_SHADER_BYTECODE{ s.pShaderBytecode, s.BytecodeLength };
	}

	D3D12_STREAM_OUTPUT_DESC Convert(const FStreamOutputDesc& s) {
		return D3D12_STREAM_OUTPUT_DESC{
			(const D3D12_SO_DECLARATION_ENTRY*)s.pSODeclaration, s.NumEntries,
			s.pBufferStrides, s.NumStrides,
			s.RasterizedStream };
	}

	D3D12_BLEND Convert(const EBlend s) {
		return (D3D12_BLEND)s;
	}

	D3D12_BLEND_OP Convert(const EBlendOp s) {
		return (D3D12_BLEND_OP)s;
	}

	D3D12_LOGIC_OP Convert(const ELogicOp s) {
		return D3D12_LOGIC_OP(s);
	}

	D3D12_RENDER_TARGET_BLEND_DESC Convert(const FRenderTargetBlendDesc& s) {
		return D3D12_RENDER_TARGET_BLEND_DESC{ s.BlendEnable, s.LogicOpEnable,
			Convert(s.SrcBlend), Convert(s.DestBlend), Convert(s.BlendOp),
		Convert(s.SrcBlendAlpha), Convert(s.DestBlendAlpha), Convert(s.BlendOpAlpha),
		Convert(s.LogicOp), s.RenderTargetWriteMask };
	}

	D3D12_BLEND_DESC Convert(const FBlendDesc& s) {
		D3D12_BLEND_DESC desc{ s.AlphaToCoverageEnable, s.IndependentBlendEnable };
		for (int i = 0; i < FB_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i) {
			desc.RenderTarget[i] = Convert(s.RenderTarget[i]);
		}
		return desc;
	}

	D3D12_FILL_MODE Convert(EFillMode s) {
		return (D3D12_FILL_MODE)s;
	}

	D3D12_CULL_MODE Convert(ECullMode s) {
		return (D3D12_CULL_MODE)s;
	}
	D3D12_CONSERVATIVE_RASTERIZATION_MODE Convert(EConservativeRasterizationMode s) {
		return D3D12_CONSERVATIVE_RASTERIZATION_MODE(s);
	}

	D3D12_RASTERIZER_DESC Convert(const FRasterizerDesc& s) {
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

	D3D12_DEPTH_WRITE_MASK Convert(EDepthWriteMask s) {
		return D3D12_DEPTH_WRITE_MASK(s);
	}

	D3D12_COMPARISON_FUNC Convert(EComparisonFunc s) {
		return D3D12_COMPARISON_FUNC(s);
	}

	D3D12_DEPTH_STENCILOP_DESC Convert(const FDepthStencilOpDesc& s) {

	}

	D3D12_DEPTH_STENCIL_DESC Convert(const FDepthStencilDesc& s) {
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

	D3D12_GRAPHICS_PIPELINE_STATE_DESC Convert(const FPSODesc& s)
	{
		D3D12_GRAPHICS_PIPELINE_STATE_DESC desc = {
			(ID3D12RootSignature*)s.pRootSignature,
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




		}
	}
}