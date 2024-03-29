#pragma once
#include "IRenderer.h"
namespace fb
{	
	enum class RendererType {
		D3D12,
		VULKAN,
		D3D11,
		OPENGL
	};

	IRenderer* InitRenderer(RendererType type, void* windowHandle);
}
