#include "RenderResources.h"
#include "D3DCreateHelper.h"
#include "D3DDescriptorHeap.h"
#include "RootSignatureDescBundle.h"
#include "SubObjects/SubObjectsCreate.h"
#include "SubObjects/SubObject.h"
#include "PipelineState/RtPipelineState.h"
#include "Shader/DirectXCompiler.h"
#include "Shader/DxilEntryPoints.h"
#include "Shader/ShaderTable.h"
#include "D3DHeplerFunction.h"
#include "DxilLibrarySubobjects.h"
#include <d3d12.h>
#include <dxgi1_5.h>

const unsigned int SWAP_CHAIN_BUFFER_COUNT = 2U;

RenderResources::RenderResources() : fenceValue(0), fenceEvent(nullptr), hwnd(nullptr), size{}
{
}

void RenderResources::Init(HWND hwnd, unsigned int wndWidth, unsigned int wndHeight)
{
	size = { wndWidth, wndHeight };
	InitFactory4();
	InitDevice();
	InitGraphicsCommandList4();
	InitCommandQueue();
	InitDescriptorHeaps();
	InitOutputResource();
	InitSwapChain(hwnd, size.width, size.height);
	InitFence();
	InitFenceEvent();
	InitPipelineState();
	InitShaderTable();
}

void RenderResources::InitFactory4()
{
	factory = d3d_create_helper::CreateDxgiFactory4();
}

void RenderResources::InitDevice()
{
	device = d3d_create_helper::CreateDevice5(factory);
}

void RenderResources::InitGraphicsCommandList4()
{
	cmdAllocator = d3d_create_helper::CreateCommandAllocator(device, D3D12_COMMAND_LIST_TYPE_DIRECT);
	cmdList = d3d_create_helper::CreateGraphicsCommandList4(device, cmdAllocator, D3D12_COMMAND_LIST_TYPE_DIRECT);
}

void RenderResources::InitCommandQueue()
{
	cmdQueue = d3d_create_helper::CreateCommandQueue(device, D3D12_COMMAND_LIST_TYPE_DIRECT);
}

void RenderResources::InitSwapChain(HWND& hwnd, unsigned int width, unsigned int height)
{
	swapChain = d3d_create_helper::CreateSwapChain(factory, cmdQueue, hwnd, width, height, DXGI_FORMAT_R8G8B8A8_UNORM);
	InitRenderTarget();
	rtvHandles.resize(SWAP_CHAIN_BUFFER_COUNT);
	auto desc = d3d_create_helper::CreateRenderTargetViewDesc(DXGI_FORMAT_R8G8B8A8_UNORM);
	for (unsigned int i = 0; i < SWAP_CHAIN_BUFFER_COUNT; ++i)
	{
		rtvBuffers[i].heapIndex = heapRtv->AddViewDesc(desc, rtvBuffers[i].resource);
	}

	heapRtv->CreateViews(false);

	for (unsigned int i = 0; i < SWAP_CHAIN_BUFFER_COUNT; ++i)
	{
		rtvHandles[i] = heapRtv->GetCpuHandle(rtvBuffers[i].heapIndex);
	}
}

void RenderResources::InitFence()
{
	fence = d3d_create_helper::CreateFence(device);
}

void RenderResources::InitFenceEvent()
{
	fenceEvent = CreateEvent(nullptr, false, false, nullptr);
}

void RenderResources::InitRenderTarget()
{
	for (unsigned int i = 0; i < SWAP_CHAIN_BUFFER_COUNT; ++i)
	{
		swapChain->GetBuffer(i, IID_PPV_ARGS(&rtvBuffers[i].resource));
	}
}

void RenderResources::InitDescriptorHeaps()
{
	heapCSU = std::make_shared<D3DDescriptorHeap>(device);
	heapRtv = std::make_unique<D3DDescriptorHeap>(device);
}

void RenderResources::InitOutputResource()
{
	outputResource.resource = d3d_create_helper::CreateTexture2D(device,
		size.height, size.width,
		D3D12_TEXTURE_LAYOUT_UNKNOWN,
		DXGI_FORMAT_R8G8B8A8_UNORM,
		D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);
	outputResource.resource->SetName(L"outputResoruce");

	d3d_helper_function::TlansitoinBarrier(cmdList, outputResource.resource, D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_SOURCE);
}

void RenderResources::InitShaderPair()
{
	rayGenName = L"rayGen";
	missSets.push_back({ 0, L"miss" });
}

void RenderResources::InitPipelineState()
{
	InitShaderPair();

	pipeline = std::make_shared<RtPipelineState>();

	auto dxlilSubobject = CompileDxilLibrarySubobject();
	pipeline->AddSubObject(dxlilSubobject);

	pipeline->CreatePipelineState(device);
}

void RenderResources::AddRootSignature(std::shared_ptr<RtPipelineState>& pipeline, const std::vector<RootSigShaderBundle>& root)
{
	for (auto& r : root)
	{
		auto rootIndex = pipeline->AddSubObject(r.rootSig);
		auto association = sub_objects::CreateExportAssociation(r.associateShaderNames, rootIndex);
		pipeline->AddSubObject(association);
	}
}

void RenderResources::InitShaderTable()
{
	shaderTable = std::make_shared<ShaderTable>();

	ShaderTable::ShaderInfo info;

	//ここはおそらく固定で参照するリソースもないのでここでやっとく？今後変更があるかもしれない
	info.heapIndices.clear();
	for (auto& miss : missSets)
	{
		info.name = miss.second;
		shaderTable->AddMissShader(info);
	}
}

std::shared_ptr<DxilLibrarySubobjects> RenderResources::CompileDxilLibrarySubobject()
{
	const std::wstring SHADER_PATH = L"hlsl/RGShader.hlsl";
	const std::wstring TARGET = L"lib_6_3";
	DirectXCompiler compiler;
	auto blob = compiler.Compile(SHADER_PATH, TARGET);

	auto hr = device->CreateRootSignature(0, blob->GetBufferPointer(), blob->GetBufferSize(), IID_PPV_ARGS(rootSignature.GetAddressOf()));
	return std::make_shared<DxilLibrarySubobjects>(blob);
}

