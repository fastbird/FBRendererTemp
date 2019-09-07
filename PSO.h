#pragma once
#include "Types.h"
#include "InputElementDesc.h"
#include "../FBCommon/Types.h"
#include "PrimitiveTopology.h"
#define	FB_SIMULTANEOUS_RENDER_TARGET_COUNT	( 8 )
#define	FB_DEFAULT_STENCIL_READ_MASK	( 0xff )
#define	FB_DEFAULT_STENCIL_WRITE_MASK	( 0xff )
// Pipeline State Object

namespace fb {
	struct FSODeclarationEntry
	{
		UINT Stream;
		const char* SemanticName;
		UINT SemanticIndex;
		BYTE StartComponent;
		BYTE ComponentCount;
		BYTE OutputSlot;
	};

	struct FStreamOutputDesc
	{
		const FSODeclarationEntry* pSODeclaration = nullptr;
		UINT NumEntries = 0;
		const UINT* pBufferStrides = nullptr;
		UINT NumStrides = 0;
		UINT RasterizedStream = 0;
	};

	enum class EBlend
	{
		ZERO = 1,
		ONE = 2,
		SRC_COLOR = 3,
		INV_SRC_COLOR = 4,
		SRC_ALPHA = 5,
		INV_SRC_ALPHA = 6,
		DEST_ALPHA = 7,
		INV_DEST_ALPHA = 8,
		DEST_COLOR = 9,
		INV_DEST_COLOR = 10,
		SRC_ALPHA_SAT = 11,
		BLEND_FACTOR = 14,
		INV_BLEND_FACTOR = 15,
		SRC1_COLOR = 16,
		INV_SRC1_COLOR = 17,
		SRC1_ALPHA = 18,
		INV_SRC1_ALPHA = 19
	};

	enum class EBlendOp
	{
		ADD = 1,
		SUBTRACT = 2,
		REV_SUBTRACT = 3,
		MIN = 4,
		MAX = 5
	};

	enum class ELogicOp
	{
		CLEAR = 0,
		SET,
		COPY,
		COPY_INVERTED,
		NOOP,
		INVERT,
		AND,
		NAND,
		OR,
		NOR,
		XOR,
		EQUIV,
		AND_REVERSE,
		AND_INVERTED,
		OR_REVERSE,
		OR_INVERTED
	};

	namespace EColorWriteEnable
	{
		enum Enum
		{
			RED = 1,
			GREEN = 2,
			BLUE = 4,
			ALPHA = 8,
			ALL = (((RED | GREEN) | BLUE) | ALPHA)
		};
	}

	struct FRenderTargetBlendDesc
	{
		FRenderTargetBlendDesc()
			: BlendEnable(false)
			, LogicOpEnable(false)
			, SrcBlend(EBlend::ONE), DestBlend(EBlend::ZERO), BlendOp(EBlendOp::ADD)
			, SrcBlendAlpha(EBlend::ONE), DestBlendAlpha(EBlend::ZERO), BlendOpAlpha(EBlendOp::ADD)
			, LogicOp(ELogicOp::NOOP)
			, RenderTargetWriteMask(EColorWriteEnable::ALL)
		{

		}

		BOOL BlendEnable;
		BOOL LogicOpEnable;
		EBlend SrcBlend;
		EBlend DestBlend;
		EBlendOp BlendOp;
		EBlend SrcBlendAlpha;
		EBlend DestBlendAlpha;
		EBlendOp BlendOpAlpha;
		ELogicOp LogicOp;
		UINT8 RenderTargetWriteMask;
	};

	struct FBlendDesc
	{
		FBlendDesc()
		{
			AlphaToCoverageEnable = false;
			IndependentBlendEnable = false;

			const FRenderTargetBlendDesc defaultRenderTargetBlendDesc;
			for (UINT i = 0; i < FB_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i)
				RenderTarget[i] = defaultRenderTargetBlendDesc;
		}

		bool AlphaToCoverageEnable;
		bool IndependentBlendEnable;
		FRenderTargetBlendDesc RenderTarget[FB_SIMULTANEOUS_RENDER_TARGET_COUNT];
	};

	enum EFillMode
	{
		WIREFRAME = 2,
		SOLID = 3
	};

	enum ECullMode
	{
		NONE = 1,
		FRONT = 2,
		BACK = 3
	};

	enum class EConservativeRasterizationMode
	{
		OFF = 0,
		ON = 1
	};

	struct FRasterizerDesc
	{
		FRasterizerDesc()
		{
			FillMode = EFillMode::SOLID;
			CullMode = ECullMode::BACK;
			FrontCounterClockwise = false;
			DepthBias = 0;
			DepthBiasClamp = 0.0f;
			SlopeScaledDepthBias = 0.0f;
			DepthClipEnable = true;
			MultisampleEnable = false;
			AntialiasedLineEnable = false;
			ForcedSampleCount = 0;
			ConservativeRaster = EConservativeRasterizationMode::OFF;
		}
		EFillMode FillMode;
		ECullMode CullMode;
		BOOL FrontCounterClockwise;
		INT DepthBias;
		FLOAT DepthBiasClamp;
		FLOAT SlopeScaledDepthBias;
		BOOL DepthClipEnable;
		BOOL MultisampleEnable;
		BOOL AntialiasedLineEnable;
		UINT ForcedSampleCount;
		EConservativeRasterizationMode ConservativeRaster;
	};

	enum class EDepthWriteMask
	{
		ZERO = 0,
		ALL = 1
	};

	enum class EComparisonFunc
	{
		NEVER = 1,
		LESS = 2,
		EQUAL = 3,
		LESS_EQUAL = 4,
		GREATER = 5,
		NOT_EQUAL = 6,
		GREATER_EQUAL = 7,
		ALWAYS = 8
	};

	enum class EStencilOp
	{
		KEEP = 1,
		ZERO = 2,
		REPLACE = 3,
		INCR_SAT = 4,
		DECR_SAT = 5,
		INVERT = 6,
		INCR = 7,
		DECR = 8
	};

	struct FDepthStencilOpDesc
	{
		FDepthStencilOpDesc()
		{
			StencilFailOp = EStencilOp::KEEP;
			StencilDepthFailOp = EStencilOp::KEEP;
			StencilPassOp = EStencilOp::KEEP;
			StencilFunc = EComparisonFunc::ALWAYS;
		}
		EStencilOp StencilFailOp;
		EStencilOp StencilDepthFailOp;
		EStencilOp StencilPassOp;
		EComparisonFunc StencilFunc;
	};

	struct FDepthStencilDesc
	{
		FDepthStencilDesc()
		{
			DepthEnable = true;
			DepthWriteMask = EDepthWriteMask::ALL;
			DepthFunc = EComparisonFunc::LESS;
			StencilEnable = false;
			StencilReadMask = FB_DEFAULT_STENCIL_READ_MASK;
			StencilWriteMask = FB_DEFAULT_STENCIL_WRITE_MASK;
		}
		BOOL DepthEnable;
		EDepthWriteMask DepthWriteMask;
		EComparisonFunc DepthFunc;
		BOOL StencilEnable;
		UINT8 StencilReadMask;
		UINT8 StencilWriteMask;
		FDepthStencilOpDesc FrontFace;
		FDepthStencilOpDesc BackFace;
	};

	enum class EIndexBufferStripCutValue
	{
		CUT_VALUE_DISABLED = 0,
		CUT_VALUE_0xFFFF = 1,
		CUT_VALUE_0xFFFFFFFF = 2
	};

	enum class EPrimitiveTopologyType
	{
		UNDEFINED = 0,
		POINT = 1,
		LINE = 2,
		TRIANGLE = 3,
		PATCH = 4
	};

	struct FSampleDesc
	{
		UINT Count = 1;
		UINT Quality = 0;
	};

	enum class EPipelineStateFlags
	{
		NONE = 0,
		TOOL_DEBUG = 0x1
	};

	struct FShaderByteCode
	{
		const void* pShaderBytecode = nullptr;
		UINT BytecodeLength = 0;
	};

	using RootSignature = void*;
	struct FPSODesc
	{
		RootSignature pRootSignature;
		FShaderByteCode VS;
		FShaderByteCode PS;
		FShaderByteCode DS;
		FShaderByteCode HS;
		FShaderByteCode GS;
		FStreamOutputDesc StreamOutput;
		FBlendDesc BlendState;
		UINT SampleMask = UINT_MAX;
		FRasterizerDesc RasterizerState;
		FDepthStencilDesc DepthStencilState;
		FInputLayoutDesc InputLayout;
		EIndexBufferStripCutValue IBStripCutValue = EIndexBufferStripCutValue::CUT_VALUE_DISABLED;
		EPrimitiveTopologyType PrimitiveTopologyType = EPrimitiveTopologyType::UNDEFINED;
		UINT NumRenderTargets = 0;
		EDataFormat RTVFormats[8] = { EDataFormat::UNKNOWN, EDataFormat::UNKNOWN, EDataFormat::UNKNOWN , EDataFormat::UNKNOWN,
			EDataFormat::UNKNOWN, EDataFormat::UNKNOWN, EDataFormat::UNKNOWN, EDataFormat::UNKNOWN};
		EDataFormat DSVFormat = EDataFormat::UNKNOWN;
		FSampleDesc SampleDesc;
		UINT NodeMask = 0;
		ByteArray CachedPSO;
		EPipelineStateFlags Flags = EPipelineStateFlags::NONE;
	};
}
