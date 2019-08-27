// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

// add headers that you want to pre-compile here
#include "framework.h"

#include <windows.h>
#include <wrl.h>
#include <dxgi1_4.h>
#include <d3d12.h>
#include <DirectXColors.h>
#include <DirectXPackedVector.h>
#include <d3dcompiler.h>
#include "d3dx12.h"
#include <string>
#include <assert.h>
#include <vector>

#define FBRendererD3D12_DLL _declspec(dllexport)

#endif //PCH_H
