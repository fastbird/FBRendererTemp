#include "pch.h"
#include "IndexBuffer.h"
#include "ConverterD3D12.h"

using namespace fb;

bool IndexBuffer::Initialize(const void* indexData, UINT size, EDataFormat format, bool keepData)
{
	Format = Convert(format);
	return GPUBuffer::Initialize(indexData, size, keepData);
}