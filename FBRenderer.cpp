#include "FBRenderer.h"
#include "IRenderer.h"
#include <exception>

// for windows
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

using namespace fb;

typedef fb::IRenderer* (*CreateRendererD3D12)();

int MyFunction1(float parameter)
{
	return (int)(parameter * 10.0);
}


IRenderer* fb::InitRenderer(RendererType type, void* windowHandle)
{
	switch (type)
	{
	case RendererType::D3D12:
	{
		HMODULE hmodule = LoadLibrary("FBRendererD3D12.dll");
		if (!hmodule)
			throw std::exception("Cannot load FBRendererD3D12.dll");
		auto createFunc = (CreateRendererD3D12)GetProcAddress(hmodule, "CreateRendererD3D12");
		auto renderer = createFunc();
		if (!renderer)
			return nullptr;
		renderer->Initialize(windowHandle);
		return renderer;

	}
	}
	return nullptr;
}