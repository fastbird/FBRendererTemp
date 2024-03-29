// RendererTestApp.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "RendererTestApp.h"
#include "../FBRenderer.h"
#include "../../FBCommon/glm.h"
#include <chrono>
#include <thread>
#include <string>
#include <array>
#include <map>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "../AxisRenderer.h"
#pragma comment(lib,"assimp-vc142-mtd.lib")

#define MAX_LOADSTRING 100

// Global Variables:
fb::IRenderer* gRenderer = nullptr;
struct MeshGeometry* gBoxMesh = nullptr;
struct MeshGeometry* FaceMesh = nullptr;

// Expected initial eye coordinates is (0, 0, -10)
float Radius = 10.0f;
float Phi = -glm::half_pi<float>();
float Theta = glm::half_pi<float>();
//float Phi = glm::four_over_pi<float>();
//float Theta = 1.5f * glm::pi<float>();
glm::mat4 WorldMat(1.0f), ViewMat(1.0f), ProjMat(1.0f);
fb::IUploadBuffer* ConstantBuffer = nullptr;
POINT LastMousePos;
fb::PSOID SimpleBoxPSO;
fb::PSOID FacePSO;
fb::AxisRenderer* gAxisRenderer = nullptr;

HINSTANCE hInst;                                // current instance
HWND WindowHandle;
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

fb::IShaderIPtr VS, PS;
fb::IShaderIPtr VS_Face;
std::vector<fb::FInputElementDesc> InputLayout;
std::vector<fb::FInputElementDesc> InputLayout_Face;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
bool				BuildBoxGeometry();
void Update(float dt);
void OnMouseMove(WPARAM btnState, int x, int y);
void OnMouseDown(WPARAM btnState, int x, int y);
void OnMouseUp(WPARAM btnState, int x, int y);
void BuildShadersAndInputLayout();
void BuildPSO();
void Draw();
MeshGeometry* ImportDAE(const char* filepath);

struct Vertex
{
	float X, Y, Z;
	float R, G, B, A;
};

struct MeshGeometry
{
	// Give it a name so we can look it up by name.
	std::string Name;

	fb::IVertexBufferIPtr VertexBuffer;
	fb::IIndexBufferIPtr IndexBuffer;

	bool IsValid() const noexcept
	{
		return VertexBuffer != nullptr;
	}	
};

struct MorphMesh : public MeshGeometry
{
	fb::IUploadBufferIPtr VertexUploadBuffer;
	fb::IVertexBufferIPtr BaseGeometry;
	std::map<std::string, fb::IVertexBufferIPtr> MorphTargets;
};

void Test()
{
	
}
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.
	Test();
    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_RENDERERTESTAPP, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_RENDERERTESTAPP));

	MSG msg = { 0 };

    // Main message loop:
	while (msg.message != WM_QUIT)
	{
		// If there are Window messages then process them.
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		// Otherwise, do animation/game stuff.
		else
		{
			static auto time = std::chrono::high_resolution_clock::now();
			auto dt = std::chrono::duration<float>(std::chrono::high_resolution_clock::now() - time).count();
			Update(dt);
			if (gRenderer)
			{
				gRenderer->Draw(dt);
				using namespace std::chrono_literals;
				std::this_thread::sleep_for(10ms);
			}
		}
	}

	delete gAxisRenderer; gAxisRenderer = nullptr;
	delete gBoxMesh; gBoxMesh = nullptr;
	delete FaceMesh; FaceMesh = nullptr;
	delete ConstantBuffer; ConstantBuffer = nullptr;
	gRenderer->Finalize(); gRenderer = nullptr;

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_RENDERERTESTAPP));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_RENDERERTESTAPP);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   WindowHandle = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!WindowHandle)
   {
      return FALSE;
   }

   ShowWindow(WindowHandle, nCmdShow);
   UpdateWindow(WindowHandle);

   gRenderer = fb::InitRenderer(fb::RendererType::D3D12, (void*)WindowHandle);

   gRenderer->CreateCBVHeap(fb::ECBVHeapType::Default);
   
   gRenderer->TempResetCommandList();
   ConstantBuffer = gRenderer->CreateUploadBuffer(sizeof(float[16]), 1, true, fb::ECBVHeapType::Default);
   BuildShadersAndInputLayout();
   gRenderer->TempCreateRootSignatureForSimpleBox();
   BuildPSO();
   gRenderer->RegisterDrawCallback(Draw);
   BuildBoxGeometry();
   FaceMesh = ImportDAE("assets/DK_WOMAN_MORPHER.DAE");

   auto clientWidth = gRenderer->GetBackbufferWidth();
   auto clientHeight = gRenderer->GetBackbufferHeight();

   gAxisRenderer = new fb::AxisRenderer(gRenderer, 0, 0, clientWidth, clientHeight, { InputLayout.data(), (UINT)InputLayout.size() });
   gAxisRenderer->SetShaders(VS, PS);

   gRenderer->TempCloseCommandList(true);
   
   ProjMat = glm::perspectiveFov(0.25f * glm::pi<float>(), (float)clientWidth, (float)clientHeight, 1.0f, 1000.0f);

   return gRenderer != nullptr;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static bool Resizing = false;
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
	case WM_ENTERSIZEMOVE:
		Resizing = true;
		break;

	case WM_EXITSIZEMOVE:
		Resizing = false;
		gRenderer->OnResized();
		break;

	case WM_SIZE:
	{
		// Save the new client area dimensions.
		UINT clientWidth = LOWORD(lParam);
		UINT clientHeight = HIWORD(lParam);
		if (gRenderer)
		{
			if (!Resizing)
			{
				gRenderer->OnResized();
				ProjMat = glm::perspectiveFov(0.25f * glm::pi<float>(), (float)clientWidth, (float)clientHeight, 1.0f, 1000.0f);
			}
		}
		return 0;
	}
	case WM_MOUSEMOVE:
		OnMouseMove(wParam, LOWORD(lParam), HIWORD(lParam));
		return 0;
	case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
		OnMouseDown(wParam, LOWORD(lParam), HIWORD(lParam));
		return 0;
	case WM_LBUTTONUP:
	case WM_MBUTTONUP:
	case WM_RBUTTONUP:
		OnMouseUp(wParam, LOWORD(lParam), HIWORD(lParam));
		break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

bool BuildBoxGeometry()
{
	std::array<Vertex, 8> vertices =
	{
		Vertex{-1.0f, -1.0f, -1.0f,		1.0f, 1.0f, 1.0f, 1.0f},
		Vertex{-1.0f, +1.0f, -1.0f,		0.0f, 0.0f, 0.0f, 1.0f},
		Vertex{+1.0f, +1.0f, -1.0f,		1.0f, 0.0f, 0.0f, 1.0f},
		Vertex{+1.0f, -1.0f, -1.0f,		0.0f, 1.0f, 0.0f, 1.0f},
		Vertex{-1.0f, -1.0f, +1.0f,		0.0f, 0.0f, 1.0f, 1.0f},
		Vertex{-1.0f, +1.0f, +1.0f,		1.0f, 1.0f, 0.0f, 1.0f}, // yellow
		Vertex{+1.0f, +1.0f, +1.0f,		0.0f, 1.0f, 1.0f, 1.0f}, // cyan
		Vertex{+1.0f, -1.0f, +1.0f,		1.0f, 0.0f, 1.0f, 1.0f}, // magenta
	};

	std::array<std::uint16_t, 36> indices =
	{
		// front face
		0, 1, 2,
		0, 2, 3,

		// back face
		4, 6, 5,
		4, 7, 6,

		// left face
		4, 5, 1,
		4, 1, 0,

		// right face
		3, 2, 6,
		3, 6, 7,

		// top face
		1, 5, 6,
		1, 6, 2,

		// bottom face
		4, 0, 3,
		4, 3, 7
	};

	const UINT vbByteSize = (UINT)vertices.size() * sizeof(Vertex);
	const UINT ibByteSize = (UINT)indices.size() * sizeof(std::uint16_t);

	gBoxMesh = new MeshGeometry;
	gBoxMesh->Name = "boxGeo";
	gBoxMesh->VertexBuffer = gRenderer->CreateVertexBuffer(vertices.data(), vbByteSize, sizeof(Vertex), false);
	gBoxMesh->IndexBuffer = gRenderer->CreateIndexBuffer(indices.data(), ibByteSize, fb::EDataFormat::R16_UINT, false);
	return gBoxMesh->IsValid();
}

void PrintMatrix(const char* name, const glm::mat4& mat)
{
	OutputDebugStringA("Mat ");
	if (name)
		OutputDebugStringA(name);
	OutputDebugStringA("\n");
	char buffer[255];
	sprintf_s(buffer, "\t%.4f, %.4f, %.4f, %.4f\n", mat[0][0], mat[1][0], mat[2][0], mat[3][0]);
	OutputDebugStringA(buffer);
	sprintf_s(buffer, "\t%.4f, %.4f, %.4f, %.4f\n", mat[0][1], mat[1][1], mat[2][1], mat[3][1]);
	OutputDebugStringA(buffer);
	sprintf_s(buffer, "\t%.4f, %.4f, %.4f, %.4f\n", mat[0][2], mat[1][2], mat[2][2], mat[3][2]);
	OutputDebugStringA(buffer);
	sprintf_s(buffer, "\t%.4f, %.4f, %.4f, %.4f\n", mat[0][3], mat[1][3], mat[2][3], mat[3][3]);
	OutputDebugStringA(buffer);
	OutputDebugStringA("\n");
}

void Update(float dt)
{
	//Theta = 0.25 * glm::pi<float>();
	//Phi = 0.25 * glm::pi<float>();
	// theta : 0~pi  vertical
	// phi : 0~2pi   horizontal
	// Convert Spherical to Cartesian coordinates.
	float x = Radius * sinf(Theta) * cosf(Phi);
	float y = Radius * cosf(Theta);
	float z = Radius * sinf(Theta) * sinf(Phi);
	

	// Build the view matrix.
	glm::vec3 eyePos(x, y, z);
	glm::vec3 target(0, 0, 0);
	ViewMat = glm::lookAt(eyePos, target, glm::vec3(0, 1, 0));
	//PrintMatrix("ViewMat", ViewMat);
	if (gAxisRenderer) {
		gAxisRenderer->SetViewMat(ViewMat);
	}
	
	auto wvp = ProjMat * ViewMat * WorldMat;
	wvp = glm::transpose(wvp);

	auto float16size = sizeof(float[16]);
	assert(sizeof(wvp) == sizeof(float[16]));
	ConstantBuffer->CopyData(0, &wvp);
	// Update the constant buffer with the latest worldViewProj matrix.
}

void OnMouseMove(WPARAM btnState, int x, int y)
{
	if ((btnState & MK_LBUTTON) != 0)
	{

		// Make each pixel correspond to a quarter of a degree.
		float dx = glm::radians(static_cast<float>(x - LastMousePos.x));
		float dy = glm::radians(static_cast<float>(y - LastMousePos.y));

		// Update angles based on input to orbit camera around box.
		Phi += dx;
		Theta += dy;
		

		Theta = glm::clamp(Theta, 0.1f, glm::pi<float>() - 0.1f);
		char str[255];

		sprintf_s(str, "Theta = %f\n", Theta);
		OutputDebugStringA(str);
		sprintf_s(str, "Phi = %f\n", Phi);
		OutputDebugStringA(str);

	}
	else if ((btnState & MK_RBUTTON) != 0)
	{
		// Make each pixel correspond to 0.005 unit in the scene.
		float dx = 0.005f * static_cast<float>(x - LastMousePos.x);
		float dy = 0.005f * static_cast<float>(y - LastMousePos.y);

		// Update the camera radius based on input.
		Radius += dx - dy;

		// Restrict the radius.
		Radius = glm::clamp(Radius, 3.0f, 15.0f);
	}

	LastMousePos.x = x;
	LastMousePos.y = y;
}

void OnMouseDown(WPARAM btnState, int x, int y)
{
	LastMousePos.x = x;
	LastMousePos.y = y;


	SetCapture(WindowHandle);
}

void OnMouseUp(WPARAM btnState, int x, int y)
{
	LastMousePos.x = x;
	LastMousePos.y = y;

	ReleaseCapture();
}

void BuildShadersAndInputLayout()
{
	VS = gRenderer->CompileShader("Shaders/SimpleShader.hlsl", nullptr, 0, fb::EShaderType::VertexShader, "VS");
	VS_Face = gRenderer->CompileShader("Shaders/FaceTest.hlsl", nullptr, 0, fb::EShaderType::VertexShader, "VS");
	PS = gRenderer->CompileShader("Shaders/SimpleShader.hlsl", nullptr, 0, fb::EShaderType::PixelShader, "PS");

	InputLayout = {
		{ "POSITION", 0, fb::EDataFormat::R32G32B32_FLOAT, 0, 0, fb::EInputClassification::PerVertexData, 0 },
		{ "COLOR", 0, fb::EDataFormat::R32G32B32A32_FLOAT, 0, 12, fb::EInputClassification::PerVertexData, 0 }
	};

	InputLayout_Face = {
		{ "POSITION", 0, fb::EDataFormat::R32G32B32_FLOAT, 0, 0, fb::EInputClassification::PerVertexData, 0 },
	};
}

void BuildPSO()
{
	fb::FPSODesc psoDesc;

	psoDesc.InputLayout = { InputLayout.data(), (UINT)InputLayout.size() };
	psoDesc.pRootSignature = gRenderer->TempGetRootSignatureForSimpleBox();
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
	psoDesc.PrimitiveTopologyType = fb::EPrimitiveTopologyType::TRIANGLE;
	psoDesc.NumRenderTargets = 1;
	psoDesc.RTVFormats[0] = gRenderer->GetBackBufferFormat();
	psoDesc.SampleDesc.Count = gRenderer->GetSampleCount();
	psoDesc.SampleDesc.Quality = gRenderer->GetMsaaQuality();
	psoDesc.DSVFormat = gRenderer->GetDepthStencilFormat();
	SimpleBoxPSO = gRenderer->CreateGraphicsPipelineState(psoDesc);

	psoDesc.VS = {
		reinterpret_cast<BYTE*>(VS_Face->GetByteCode()),
		VS_Face->Size()
	};
	psoDesc.InputLayout = { InputLayout_Face.data(), (UINT)InputLayout_Face.size() };
	psoDesc.RasterizerState.FillMode = fb::EFillMode::WIREFRAME;
	FacePSO = gRenderer->CreateGraphicsPipelineState(psoDesc);
}

void Draw()
{
	gRenderer->SetPipelineState(FacePSO);
	gRenderer->TempBindDescriptorHeap(fb::ECBVHeapType::Default);
	gRenderer->TempBindRootSignature(gRenderer->TempGetRootSignatureForSimpleBox());

	gRenderer->TempBindVertexBuffer(FaceMesh->VertexBuffer);
	gRenderer->TempBindIndexBuffer(FaceMesh->IndexBuffer);
	gRenderer->SetPrimitiveTopology(fb::EPrimitiveTopology::TRIANGLELIST);
	gRenderer->TempBindRootDescriptorTable(0, fb::ECBVHeapType::Default);
	//gRenderer->TempDrawIndexedInstanced(FaceMesh->IndexBuffer->GetElementCount());

	if (gAxisRenderer) {
		gAxisRenderer->Render();
	}
}

struct PositionOnlyVertex
{
	glm::vec3 Position;

	PositionOnlyVertex() = default;
	
	PositionOnlyVertex(float x, float y, float z)
		:Position(x, y, z)
	{
	}
	
	PositionOnlyVertex(const glm::vec3& v)
		: Position(v)
	{}
	
	PositionOnlyVertex operator*(float s) const {
		return	PositionOnlyVertex(Position * s);
	}

	PositionOnlyVertex operator+(const PositionOnlyVertex& b) const
	{
		return PositionOnlyVertex(Position + b.Position);
	}

	PositionOnlyVertex operator-(const PositionOnlyVertex& b) const
	{
		return PositionOnlyVertex(Position - b.Position);
	}
};

PositionOnlyVertex Lerp(const PositionOnlyVertex& a, const PositionOnlyVertex& b, float t) {
	return a * (1.0f - t) + b * t;
}

MeshGeometry* ImportDAE(const char* filepath)
{
	Assimp::Importer importer;
	auto scene = importer.ReadFile(filepath, aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType |
		aiProcess_MakeLeftHanded |
		aiProcess_FlipWindingOrder
	);

	char msg[512];
	sprintf_s(msg, "Num Meshes : %d\n", scene->mNumMeshes);
	OutputDebugStringA(msg);
	if (scene->mNumMeshes == 0)
		return nullptr;

	auto aiMesh = scene->mMeshes[0];
	sprintf_s(msg, "Num Vertices: %d\n", aiMesh->mNumVertices);
	OutputDebugStringA(msg);
	
	auto mesh  = new MorphMesh;
	mesh->Name = "FaceMesh";
	assert(sizeof(ai_real) == sizeof(float));
	// sizeof(float) = 4 bytes
	UINT verticesSize = aiMesh->mNumVertices * sizeof(float) * 3;
	mesh->VertexBuffer = gRenderer->CreateVertexBuffer(nullptr, 0, 0, false);
	mesh->BaseGeometry = gRenderer->CreateVertexBuffer(aiMesh->mVertices, verticesSize, sizeof(float) * 3, true);
	if (aiMesh->mNumVertices < 0xffff) {
		std::vector<uint16_t> indices;
		for (UINT i = 0; i < aiMesh->mNumFaces; ++i) {
			auto face = aiMesh->mFaces[i];
			assert(face.mNumIndices == 3);
			for (int f = 0; f < 3; ++f) {
				indices.push_back(face.mIndices[f]);
			}
		}
		mesh->IndexBuffer = gRenderer->CreateIndexBuffer(indices.data(), (UINT)indices.size() * sizeof(uint16_t), fb::EDataFormat::R16_UINT, false);
	}
	else {
		std::vector<uint32_t> indices;
		for (UINT i = 0; i < aiMesh->mNumFaces; ++i) {
			auto face = aiMesh->mFaces[i];
			assert(face.mNumIndices == 3);
			for (int f = 0; f < 3; ++f) {
				indices.push_back(face.mIndices[f]);
			}
		}
		mesh->IndexBuffer = gRenderer->CreateIndexBuffer(indices.data(), (UINT)indices.size() * sizeof(uint32_t), fb::EDataFormat::R16_UINT, false);
	}

	// Load morph targets
	for (UINT i = 0; i < aiMesh->mNumAnimMeshes; ++i) {
		auto aiAnimMesh = aiMesh->mAnimMeshes[i];
		UINT vSize = aiAnimMesh->mNumVertices * sizeof(float) * 3;
		assert(vSize == verticesSize);
		auto vertexBuffer = gRenderer->CreateVertexBuffer(aiAnimMesh->mVertices, vSize, (UINT)sizeof(float) * 3, true);
		mesh->MorphTargets.insert(std::make_pair(aiAnimMesh->mName.C_Str(), vertexBuffer));
	}

	mesh->VertexUploadBuffer = gRenderer->CreateUploadBuffer(sizeof(float) * 3, aiMesh->mNumVertices, false, fb::ECBVHeapType::None);
	float weights[] = { 0.1f, 0.2f, 0.3f, 0.4f, 0.5f, 0.6f, 0.7f, 0.8f, 0.9f, 1.0f };
	
	std::vector<PositionOnlyVertex> morphResult(aiMesh->mNumVertices);
	auto baseVertices = (PositionOnlyVertex*)mesh->BaseGeometry->GetCPUAddress();
	memcpy(morphResult.data(), mesh->BaseGeometry->GetCPUAddress(), verticesSize);
	auto usingMorphTargets = 10;
	int i = 0;
	for (auto it : mesh->MorphTargets) {
		auto name = it.first;
		auto vertexBuffer = it.second;
		auto w = weights[i];
		auto morphVertices = (PositionOnlyVertex*)vertexBuffer->GetCPUAddress();
		for (UINT v = 0; v < aiMesh->mNumVertices; ++v) {
			morphResult[v] = morphResult[v] + ((morphVertices[v] - baseVertices[v]) * w);
		}
		++i;
		if (i >= usingMorphTargets)
			break;
	}
	mesh->VertexUploadBuffer->CopyData(0, morphResult.data(), aiMesh->mNumVertices);
	
	mesh->VertexBuffer->FromUploadBuffer(mesh->VertexUploadBuffer);

	return mesh;	
}