#pragma once
#include <vector>
#include "Types.h"
#include "../FBCommon/IRefCounted.h"
#include "DataFormat.h"
#include "InputElementDesc.h"
#include "IShader.h"
#include "PSO.h"

namespace fb
{
	enum class CBVHeapType {
		None,
		Default
	};

	FBDeclareIntrusivePointer(IVertexBuffer);
	class IVertexBuffer : public IRefCounted
	{
	public:
		virtual ~IVertexBuffer() {}
		virtual bool Initialize(const void* vertexData, UINT size, UINT stride, bool keepData) = 0;
		virtual UINT GetSize() const = 0;
		virtual UINT GetStride() const = 0;
	};
	FBDeclareIntrusivePointer2(IVertexBuffer);


	FBDeclareIntrusivePointer(IIndexBuffer);
	class IIndexBuffer : public IRefCounted
	{
	public:
		virtual ~IIndexBuffer() {}
		virtual bool Initialize(const void* indexData, UINT size, EDataFormat format, bool keepData) = 0;
		virtual UINT GetSize() const = 0;
		virtual EDataFormat GetFormat() const = 0;
	};
	FBDeclareIntrusivePointer2(IIndexBuffer);

	FBDeclareIntrusivePointer(IUploadBuffer);
	class IUploadBuffer : public IRefCounted
	{
	public:
		virtual ~IUploadBuffer() {}
		// align : 256 for constant buffers. 0 for otherwise.
		virtual bool Initialize(UINT elementSize, UINT align, UINT count) = 0;
		virtual void CopyData(UINT elementIndex, void* elementData) = 0;
	};
	FBDeclareIntrusivePointer2(IUploadBuffer);

	class IRenderer
	{
	public:
		virtual bool Initialize(void* windowHandle) = 0;
		virtual void Finalize() = 0;

		virtual void OnResized() = 0;

		virtual void Draw(float dt) = 0;

		virtual IVertexBuffer* CreateVertexBuffer(const void* vertexData, UINT size, UINT stride, bool keepData) = 0;
		virtual IIndexBuffer* CreateIndexBuffer(const void* indexData, UINT size, EDataFormat format, bool keepData) = 0;
		virtual IUploadBuffer* CreateUploadBuffer(UINT elementSize, UINT count, bool constantBuffer, CBVHeapType heapType) = 0;
		virtual PSOID CreateGraphicsPipelineState(const FPSODesc& psoDesc) = 0;
		virtual IShader* CompileShader(const char* filepath, FShaderMacro* macros, int numMacros, EShaderType shaderType, const char* entryFunctionName) = 0;
		virtual EDataFormat GetBackBufferFormat() const = 0;
		virtual EDataFormat GetDepthStencilFormat() const = 0;
		virtual int GetSampleCount() const = 0;
		virtual int GetMsaaQuality() const = 0;

		virtual void TestCreateRootSignatureForSimpleBox() = 0;
		virtual RootSignature* TestGetRootSignatureForSimpleBox() = 0;
	};
}

