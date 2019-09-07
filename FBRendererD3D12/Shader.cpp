#include "pch.h"
#include "Shader.h"

using namespace fb;

BYTE* Shader::GetByteCode()
{
	if (!ByteCode)
		return nullptr;
	return (BYTE*)ByteCode->GetBufferPointer();
}

UINT Shader::Size()
{
	if (!ByteCode)
		return 0;
	return ByteCode->GetBufferSize();
}