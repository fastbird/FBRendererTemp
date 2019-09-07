#pragma once
#include <string>
#include "Types.h"
#include "DataFormat.h"

namespace fb
{
	enum class EInputClassification
	{
		PerVertexData,
		PerInstanceData
	};

	struct FInputElementDesc
	{
		const char* SemanticName;
		UINT SemanticIndex; // UV0? UV1?
		EDataFormat Format;
		UINT InputSlot; // index of the vertex buffer. In case we use multiple vertex buffers to form a complete vertices stream.
		UINT AlignedByteOffset;
		EInputClassification InputSlotClass;
		UINT InstanceDataStepRate;
	};

	struct FInputLayoutDesc
	{
		const FInputElementDesc* pInputElementDescs = nullptr;
		UINT NumElements = 0;
	};
}