#pragma once
#include "../FBCommon/glm.h"

namespace fb
{
	class AxisRenderer
	{
		int PosX, PosY;
		int Width, Height;
		fb::IShaderIPtr VS, PS;
		fb::PSOID PipelineStateId;
		IRenderer* Renderer;
		IRootSignatureIPtr RootSignature;
		fb::IVertexBufferIPtr VB;
		glm::mat4 ViewProj;
		glm::mat4 Proj;
		glm::mat4 View;
		FInputLayoutDesc InputLayoutDesc;
	public:
		AxisRenderer(IRenderer* renderer, int x, int y, int width, int height, const FInputLayoutDesc& inputLayoutDesc);
		void SetShaders(fb::IShaderIPtr vs, fb::IShaderIPtr ps);
		void SetViewMat(const glm::mat4& viewMat);
		void Render();
	};
}