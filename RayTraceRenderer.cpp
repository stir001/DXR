#include "RayTraceRenderer.h"
#include "D3DCreateHelper.h"
#include "D3DDescriptorHeap.h"
#include "RootSignatureDescBundle.h"
#include "Shader/ShaderTable.h"
#include "SubObjects/SubObject.h"
#include "PipelineState/RtPipelineState.h"
#include "D3DHeplerFunction.h"
#include "DrawObjectCreator.h"
#include "Camera.h"
#include "GameObject.h"
#include <d3d12.h>
#include <dxgi1_5.h>
#include <string>

const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;

LRESULT WinProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		break;
	}

	return DefWindowProcA(hwnd, msg, wParam, lParam);
}

RayTraceRenderer::RayTraceRenderer(HINSTANCE hinst) :mHwnd(), mHInst(hinst)
{
	Init();
}

RayTraceRenderer::~RayTraceRenderer()
{
}

void RayTraceRenderer::Init()
{
	InitWindow();
	mResources.Init(mHwnd, WINDOW_WIDTH, WINDOW_HEIGHT);
	CreateHitGroupPatterns();
	mCreator = std::make_shared<DrawObjectCreator>(*mResources.heapCSU, mResources.device, mResources.cmdList, mResources.shaderTable);

	//GameObjectInitalize
	Vector3 origin = {};
	float moveLength = 3.0f;
	mTestObjects.push_back(mCreator->LoadFMD("model/King.fmd"));
	mTestObjects.back()->SetPos(origin);
	mTestObjects.push_back(mCreator->LoadFMD("model/Bishop.fmd"));
	mTestObjects.back()->SetPos(origin + Vector3(moveLength, 0.0f, 0.0f));
	mTestObjects.push_back(mCreator->LoadFMD("model/Knight.fmd"));
	mTestObjects.back()->SetPos(origin + Vector3(-moveLength, 0.0f, 0.0f));
	mTestObjects.push_back(mCreator->LoadFMD("model/pawn.fmd"));
	mTestObjects.back()->SetPos(origin + Vector3(moveLength * 2.0f, 0.0f, 0.0f));
	mTestObjects.push_back(mCreator->LoadFMD("model/Queen.fmd"));
	mTestObjects.back()->SetPos(origin + Vector3(-moveLength * 2.0f, 0.0f, 0.0f));
	mTestObjects.push_back(mCreator->LoadFMD("model/Rook.fmd"));
	auto tlas = mCreator->CreateTLAS();

	//outputresourceÇÃí«â¡
	auto tex2DDesc = d3d_create_helper::CreateSRVTexture2D(DXGI_FORMAT_R8G8B8A8_UNORM);
	tex2DDesc.Texture2D.PlaneSlice = 0;
	mResources.outputResource.heapIndex = mResources.heapCSU->AddViewDesc(tex2DDesc, mResources.outputResource.resource);

	//TLASÇÃí«â¡
	auto desc = d3d_create_helper::CreateSRVAS(tlas.result);
	auto tlasIndex =  mResources.heapCSU->AddViewDesc(desc, nullptr);

	//ÉJÉÅÉâÇÃçÏê¨
	mCamera = std::make_shared<Camera>(mResources.device, *mResources.heapCSU, static_cast<float>(mResources.size.width) / static_cast<float>(mResources.size.height));
	mCamera->SetPos(Vector3(0.0f, 0.0f, -20.0f));

	//rayGenerationShaderÇÃìoò^
	ShaderTable::ShaderInfo info = {};
	info.heapIndices.push_back(mResources.outputResource.heapIndex);
	info.name = mResources.rayGenName;
	mResources.shaderTable->AddRayGenShader(info);

	mResources.heapCSU->CreateViews();

	ShaderTable::InitStructure init;
	init.device = mResources.device;
	init.heap = mResources.heapCSU;
	init.pipelineState = mResources.pipeline;
	mResources.shaderTable->Create(init);

	ExcuteCommandList(mResources.cmdList);
	WaitFence();
	ResetCommandList();
}

void RayTraceRenderer::BeginFrame()
{
	auto& cmdList = mResources.cmdList;
	mResources.heapCSU->SetHeap(cmdList);
}

void RayTraceRenderer::Render()
{
	mCreator->UpdateTLAS();

	auto& cmdList = mResources.cmdList;

	auto& outBuffer = mResources.outputResource.resource;
	d3d_helper_function::TlansitoinBarrier(cmdList, outBuffer,
		D3D12_RESOURCE_STATE_COPY_SOURCE, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);

	DispatchRays();

	d3d_helper_function::TlansitoinBarrier(cmdList, outBuffer,
		D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_COPY_SOURCE);
	unsigned int backBufferIndex = mResources.swapChain->GetCurrentBackBufferIndex();
	auto& backBuffer = mResources.rtvBuffers[backBufferIndex].resource;
	d3d_helper_function::TlansitoinBarrier(cmdList, backBuffer,
		D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_COPY_DEST);

	cmdList->CopyResource(backBuffer.Get(), outBuffer.Get());

	d3d_helper_function::TlansitoinBarrier(cmdList, backBuffer,
		D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PRESENT);

	ExcuteCommandList(mResources.cmdList);
	SwapChainPresent();
	WaitFence();
	ResetCommandList();
}

void RayTraceRenderer::WaitFence()
{
	mResources.fence->SetEventOnCompletion(mResources.fenceValue, mResources.fenceEvent);
	WaitForSingleObject(mResources.fenceEvent, INFINITE);
	unsigned int count = 0;
	while (!(mResources.fence->GetCompletedValue() >= mResources.fenceValue))
	{
		++count;
	}
}

void RayTraceRenderer::ExcuteCommandList(const MWCptr<ID3D12GraphicsCommandList>& cmdList)
{
	cmdList->Close();
	ID3D12CommandList* pList = cmdList.Get();
	mResources.cmdQueue->ExecuteCommandLists(1, &pList);
	++mResources.fenceValue;
	mResources.cmdQueue->Signal(mResources.fence.Get(), mResources.fenceValue);
}

void RayTraceRenderer::SetEvent()
{
	mResources.fence->SetEventOnCompletion(mResources.fenceValue, mResources.fenceEvent);
}

unsigned int RayTraceRenderer::GetCurrentBackBufferIndex() const
{
	return mResources.swapChain->GetCurrentBackBufferIndex();
}

D3DResource RayTraceRenderer::GetRtBuffer(unsigned int index) const
{
	return mResources.rtvBuffers[index];
}

const RenderResources::WindowSize& RayTraceRenderer::GetWindowSize() const
{
	return mResources.size;
}

const MWCptr<ID3D12GraphicsCommandList4>& RayTraceRenderer::GetCommandList() const
{
	return mResources.cmdList;
}

const MWCptr<ID3D12Device5>& RayTraceRenderer::GetDevice() const
{
	return mResources.device;
}

void RayTraceRenderer::CreateViews()
{
	mResources.heapCSU->CreateViews();
}

void RayTraceRenderer::InitWindow()
{
	RECT wrc = { 0,0, WINDOW_WIDTH, WINDOW_HEIGHT };
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);
	
	CHAR name[] = ("Game");
	HINSTANCE hInst = mHInst;

	WNDCLASSEX w = {};
	w.lpfnWndProc = (WNDPROC)WinProc;
	w.lpszClassName = name;
	w.hInstance = hInst;
	w.hIcon = nullptr;
	w.hIconSm = nullptr;
	w.cbSize = sizeof(w);
	w.hCursor = LoadCursor(NULL, IDC_ARROW);
	RegisterClassEx(&w);

	auto a = 1U;

	mHwnd = CreateWindow(w.lpszClassName,
		name,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		wrc.right - wrc.left,
		wrc.bottom - wrc.top,
		nullptr,
		nullptr,
		hInst,
		nullptr);

	ShowWindow(mHwnd, SW_SHOW);
}

void RayTraceRenderer::ClearRenderTargetView()
{
	D3D12_RECT rect = {};
	rect.right = mResources.size.width;
	rect.bottom = mResources.size.height;
	float clearColor[4] = {1.0f,1.0f,1.0f,1.0f};
	mResources.cmdList->ClearRenderTargetView(mResources.rtvHandles[0], clearColor, 1, &rect);
}

void RayTraceRenderer::ResetCommandList()
{
	mResources.cmdAllocator->Reset();
	mResources.cmdList->Reset(mResources.cmdAllocator.Get(), nullptr);
}

void RayTraceRenderer::SwapChainPresent()
{
	mResources.swapChain->Present(1, 0);
}

void RayTraceRenderer::DispatchRays()
{
	const auto& table = mResources.shaderTable;
	const auto& tableEntrySize = table->GetShaderTableEntrySize();

	D3D12_DISPATCH_RAYS_DESC desc = {};
	desc.Depth = 1;
	desc.Width = mResources.size.width;
	desc.Height = mResources.size.height;

	desc.RayGenerationShaderRecord.StartAddress = table->GetRayGenTableAddress();
	desc.RayGenerationShaderRecord.SizeInBytes = static_cast<UINT64>(table->GetRayGenNum()) * tableEntrySize;

	desc.MissShaderTable.StartAddress = table->GetMissTableAddress();
	desc.MissShaderTable.StrideInBytes = tableEntrySize;
	desc.MissShaderTable.SizeInBytes = static_cast<UINT64>(table->GetMissNum()) * tableEntrySize;

	desc.HitGroupTable.StartAddress = table->GetHitGroupTableAddress();
	desc.HitGroupTable.StrideInBytes = tableEntrySize;
	desc.HitGroupTable.SizeInBytes = static_cast<UINT64>(table->GetHitGroupNum()) * tableEntrySize;

	auto& cmdList = mResources.cmdList;
	cmdList->SetComputeRootSignature(mResources.rootSignature.Get());
	cmdList->SetPipelineState1(mResources.pipeline->GetPipelineState().Get());
	mResources.cmdList->SetComputeRootDescriptorTable(0, mResources.heapCSU->GetGpuHandle(mCamera->GetCameraHeapIndex()));
	
	cmdList->DispatchRays(&desc);
}

void RayTraceRenderer::CreateHitGroupPatterns()
{
	mGlassHitPattern.resize(1);
	mPlaneHitPattern.resize(1);
}

