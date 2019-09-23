#include "pch.h"
#include "RootSignature.h"
#include "RendererD3D12.h"
using namespace fb;

void RootSignature::Bind()
{
	gRendererD3D12->Bind(RootSignature.Get());
}