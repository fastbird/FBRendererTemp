#include "FBRenderer.h"
#include "AxisRenderer.h"
#include "../FBCommon/glm.h"

using namespace fb;

template<typename T, size_t N>
size_t ArrayCount(T(&arr)[N])
{
	return N;
}

AxisRenderer::AxisRenderer(IRenderer* renderer, int x, int y, int width, int height, const FInputLayoutDesc& inputLayoutDesc)
	: PosX(x), PosY(y)
	, Width(width), Height(height)
	, Renderer(renderer)
	, InputLayoutDesc(inputLayoutDesc)
{
	RootSignature = Renderer->CreateRootSignature("RootConstant,0,16");

	struct Vertex
	{
		Vertex(float x, float y, float z, float r, float g, float b)
			: Pos(x, y, z)
			, Color(r, g, b, 1.0f)
		{}
		glm::vec3 Pos;
		glm::vec4 Color;
	};
	Vertex vertices[] = {
		Vertex(0.f, 0.f, 0.f, 1.f, 0.f, 0.f),
		Vertex(1.f, 0.f, 0.f, 1.f, 0.f, 0.f),
		Vertex(0.0f, 0.f, 0.01f, 1.f, 0.f, 0.f),
		Vertex(1.f, 0.f, 0.01f, 1.f, 0.f, 0.f),
		Vertex(0.0f, 0.f, -0.01f, 1.f, 0.f, 0.f),
		Vertex(1.f, 0.f, -0.01f, 1.f, 0.f, 0.f),


		Vertex(0.f, 0.f, 0.f, 0.f, 1.f, 0.f),
		Vertex(0.f, 1.f, 0.f, 0.f, 1.f, 0.f),
		Vertex(0.01f, 0.0f, 0.f, 0.f, 1.f, 0.f),
		Vertex(0.01f, 1.f, 0.f, 0.f, 1.f, 0.f),
		Vertex(-0.01f, 0.0f, 0.f, 0.f, 1.f, 0.f),
		Vertex(-0.01f, 1.f, 0.f, 0.f, 1.f, 0.f),

		Vertex(0.f, 0.f, 0.f, 0.f, 0.f, 1.f),
		Vertex(0.f, 0.f, 1.f, 0.f, 0.f, 1.f),
		Vertex(0.f, 0.01f, 0.f, 0.f, 0.f, 1.f),
		Vertex(0.f, 0.01f, 1.f, 0.f, 0.f, 1.f),
		Vertex(0.f, -0.01f, 0.f, 0.f, 0.f, 1.f),
		Vertex(0.f, -0.01f, 1.f, 0.f, 0.f, 1.f),
	};

	VB = Renderer->CreateVertexBuffer(vertices, sizeof(Vertex) * ArrayCount(vertices), sizeof(Vertex), false);

	glm::vec3 eyePos(0.f, 0.f, -5.0f);
	glm::vec3 target(0, 0, 0);
	auto View = glm::lookAt(eyePos, target, glm::vec3(0, 1, 0));
	Proj = glm::perspectiveFov(0.25f * glm::pi<float>(), (float)Width, (float)Height, 1.0f, 1000.0f);
	ViewProj = glm::transpose(Proj * View);
}

void AxisRenderer::SetShaders(fb::IShaderIPtr vs, fb::IShaderIPtr ps)
{
	VS = vs;
	PS = ps;

	// pso : sharable? 
	fb::FPSODesc psoDesc;
	psoDesc.InputLayout = InputLayoutDesc;
	psoDesc.pRootSignature = RootSignature;
	psoDesc.VS =
	{
		reinterpret_cast<BYTE*>(VS->GetByteCode()),
		VS->Size()
	};
	psoDesc.PS =
	{
		reinterpret_cast<BYTE*>(PS->GetByteCode()),
		PS->Size()
	};
	psoDesc.DepthStencilState.DepthEnable = false;
	psoDesc.RasterizerState.CullMode = fb::ECullMode::NONE;
	psoDesc.PrimitiveTopologyType = fb::EPrimitiveTopologyType::LINE; // Different
	psoDesc.NumRenderTargets = 1;
	psoDesc.RTVFormats[0] = Renderer->GetBackBufferFormat();
	psoDesc.SampleDesc.Count = Renderer->GetSampleCount();
	psoDesc.SampleDesc.Quality = Renderer->GetMsaaQuality();
	psoDesc.DSVFormat = Renderer->GetDepthStencilFormat();
	PipelineStateId = Renderer->CreateGraphicsPipelineState(psoDesc);
}

void PrintMatrix(const char* name, const glm::mat4& mat);
void AxisRenderer::SetViewMat(const glm::mat4& viewMat)
{
	View = viewMat;
	ViewProj = glm::transpose(Proj * viewMat);
}

void AxisRenderer::Render()
{
	Renderer->SetPipelineState(PipelineStateId);
	RootSignature->Bind();
	//  viewport&scissor
	Renderer->SetViewportAndScissor(PosX, PosY, Width, Height);

	VB->Bind(0);
	Renderer->SetPrimitiveTopology(EPrimitiveTopology::LINELIST);
	Renderer->SetGraphicsRoot32BitConstants(0, 16, &ViewProj, 0);
	Renderer->DrawInstanced(VB->GetNumVertices(), 1, 0, 0);

	auto t = glm::translate(glm::vec3(2.0f, 0.0f, 0.0f));
	auto xt = glm::transpose(Proj * View * t);
	Renderer->SetGraphicsRoot32BitConstants(0, 16, &xt, 0);
	Renderer->DrawInstanced(VB->GetNumVertices(), 1, 0, 0);
	t = glm::translate(glm::vec3( 0.0f, 4.0f, 0.0f ));
	xt = glm::transpose(Proj * View * t);
	Renderer->SetGraphicsRoot32BitConstants(0, 16, &xt, 0);
	Renderer->DrawInstanced(VB->GetNumVertices(), 1, 0, 0);

	t = glm::translate(glm::vec3(0.0f, 0.0f, 6.0f));
	xt = glm::transpose(Proj * View * t);
	Renderer->SetGraphicsRoot32BitConstants(0, 16, &xt, 0);
	Renderer->DrawInstanced(VB->GetNumVertices(), 1, 0, 0);
}