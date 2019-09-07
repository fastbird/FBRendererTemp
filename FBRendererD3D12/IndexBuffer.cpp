#include "pch.h"
#include "IndexBuffer.h"
#include "ConverterD3D12.h"

using namespace fb;

bool IndexBuffer::Initialize(const void* indexData, UINT size, EDataFormat format, bool keepData)
{
	Format = Convert(format);
	switch (format) {
	case EDataFormat::R16_UINT:
	{
		ElementCount = size / 2;
		assert((float)ElementCount == size / 2.0f);
		break;
	}
	case EDataFormat::R32_UINT:
	{
		ElementCount = size / 4;
		assert((float)ElementCount == size / 4.0f);
		break;
	}
	}
	return GPUBuffer::Initialize(indexData, size, keepData);
}

EDataFormat IndexBuffer::GetFormat() const
{
	return Convert(Format);
}