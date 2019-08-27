#pragma once
#include "../IRenderer.h"
namespace fb {
	DXGI_FORMAT Convert(EDataFormat format) {
		return (DXGI_FORMAT)format;
	}
}