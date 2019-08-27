#include "pch.h"
#include "VertexBuffer.h"
using namespace fb;

bool VertexBuffer::Initialize(const void* vertexData, UINT size, UINT stride, bool keepData)
{
	Stride = stride;
	return GPUBuffer::Initialize(vertexData, size, keepData);
}