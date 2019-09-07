#pragma once
#include "../FBCommon/IRefCounted.h"
namespace fb
{
	enum class EShaderType
	{
		PixelShader,
		VertexShader,
		GeometryShader,
		HullShader,
		DomainShader,
		ComputeShader
	};
#define FB_SHADER_MACRO_STRING_LENGTH 256
	struct FShaderMacro
	{
		char Name[FB_SHADER_MACRO_STRING_LENGTH];
		char Def[FB_SHADER_MACRO_STRING_LENGTH];
	};

	FBDeclareIntrusivePointer(IShader);
	class IShader : public IRefCounted
	{
		EShaderType ShaderType;
	public:

		virtual BYTE* GetByteCode() = 0;
		virtual UINT Size() = 0;

	};
	FBDeclareIntrusivePointer2(IShader);
}
