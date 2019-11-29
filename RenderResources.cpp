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
	InitRayGenRootSig();
	InitMissRootSig();
	InitClosestHitRootSig();
	InitGlobalRootSig();
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

	d3d_helper_function::TlansitoinBarrier(cmdList, outputResource.resource, D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_SOURCE);

	auto desc = d3d_create_helper::CreateUAVTexture2D();
	outputResource.heapIndex = heapCSU->AddViewDesc(desc, outputResource.resource, nullptr);
	outputResource.resource->SetName(L"outputResoruce");
}

void RenderResources::InitRayGenRootSig()
{
	RootSignatureDescBundle bundle = {};
	//output
	bundle.range.resize(2);
	bundle.range[0].BaseShaderRegister = 0;
	bundle.range[0].NumDescriptors = 1;
	bundle.range[0].OffsetInDescriptorsFromTableStart = 0;
	bundle.range[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;
	bundle.range[0].RegisterSpace = 0;
	//AS
	bundle.range[1].BaseShaderRegister = 0;
	bundle.range[1].NumDescriptors = 1;
	bundle.range[1].OffsetInDescriptorsFromTableStart = 1;
	bundle.range[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	bundle.range[1].RegisterSpace = 0;

	bundle.param.resize(1);
	bundle.param[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	bundle.param[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	bundle.param[0].DescriptorTable.NumDescriptorRanges = bundle.range.size();
	bundle.param[0].DescriptorTable.pDescriptorRanges = bundle.range.data();

	bundle.desc.Flags = D3D12_ROOT_SIGNATURE_FLAG_LOCAL_ROOT_SIGNATURE;
	bundle.desc.NumParameters = bundle.param.size();
	bundle.desc.pParameters = bundle.param.data();
	bundle.desc.NumStaticSamplers = 0;
	bundle.desc.pStaticSamplers = nullptr;

	rayGenRootSig = sub_objects::CreateSubObjectRootSignature(device, bundle.desc, true);
}

void RenderResources::InitMissRootSig()
{
	RootSignatureDescBundle bundle = {};
	bundle.desc.Flags = D3D12_ROOT_SIGNATURE_FLAG_LOCAL_ROOT_SIGNATURE;
	missRootSig = sub_objects::CreateSubObjectRootSignature(device, bundle.desc, true);
}

void RenderResources::InitClosestHitRootSig()
{
	RootSignatureDescBundle bundle = {};
	bundle.desc.Flags = D3D12_ROOT_SIGNATURE_FLAG_LOCAL_ROOT_SIGNATURE;
	closestHitRootSig = sub_objects::CreateSubObjectRootSignature(device, bundle.desc, true);
}

void RenderResources::InitGlobalRootSig()
{
	globalRootSig = d3d_create_helper::CreateRootSignature(device, {});
}

void RenderResources::InitPipelineState()
{
	pipeline = std::make_shared<RtPipelineState>();
	
	std::wstring rayGen = L"rayGen";
	rayGenName = rayGen;

	std::wstring anyhit = L"";
	anyHitNames;
	std::wstring closesthit = L"chs";
	closestHitNames.push_back(closesthit);
	std::wstring hitGroup = L"defaultChs";
	hitGroupNames.push_back(hitGroup);

	std::wstring miss = L"miss";
	missNames.push_back(miss);
	
	unsigned int index = 0;

	auto shader = CompileShader();
	pipeline->AddSubObject(shader);

	auto defaultHitGroup = sub_objects::CreateHitGroup(anyhit, closesthit, hitGroup);
	pipeline->AddSubObject(defaultHitGroup);

	index = pipeline->AddSubObject(rayGenRootSig);
	std::vector<std::wstring> raGenNames = { rayGenName };
	auto rayGenAssociation = sub_objects::CreateExportAssociation(raGenNames, index);
	pipeline->AddSubObject(rayGenAssociation);

	index = pipeline->AddSubObject(closestHitRootSig);
	std::vector<std::wstring> hitNames = { closesthit };
	auto closestHitAssociation = sub_objects::CreateExportAssociation(hitNames, index);
	pipeline->AddSubObject(closestHitAssociation);

	index = pipeline->AddSubObject(missRootSig);
	std::vector<std::wstring> missNames = { miss };
	auto missAssociation = sub_objects::CreateExportAssociation(missNames, index);
	pipeline->AddSubObject(missAssociation);

	auto config = sub_objects::CreateShaderConfig(sizeof(float) * 2, sizeof(float) * 3);
	index = pipeline->AddSubObject(config);
	std::vector<std::wstring> shaders = { rayGen, miss, closesthit };
	auto configAssociation = sub_objects::CreateExportAssociation(shaders, index);
	pipeline->AddSubObject(configAssociation);

	auto pipelineConfig = sub_objects::CreatePipelineConfig(1);
	pipeline->AddSubObject(pipelineConfig);

	auto global= sub_objects::CreateSubObjectRootSignature(globalRootSig, false);
	pipeline->AddSubObject(global);

	pipeline->CreatePipelineState(device);
}

const std::wstring SHADER_PATH = L"hlsl/RGShader.hlsl";
const std::wstring TARGET = L"lib_6_3";

std::shared_ptr<DxilEntryPoints> RenderResources::CompileShader()
{
	HRESULT hr = 0;
	DirectXCompiler compiler;
	auto blob = compiler.Compile(SHADER_PATH, TARGET);

	const unsigned int ENTRY_POINT_COUNT = 1 + missNames.size() + anyHitNames.size() + closestHitNames.size();

	////エントリーポイントの登録
	std::vector<std::wstring> entryPoints;
	entryPoints.reserve(ENTRY_POINT_COUNT);
	entryPoints.push_back(rayGenName);
	for (auto& name : missNames)
	{
		entryPoints.push_back(name);
	}

	for (auto& name : closestHitNames)
	{
		entryPoints.push_back(name);
	}

	for (auto& name : anyHitNames)
	{
		entryPoints.push_back(name);
	}

	std::shared_ptr<DxilEntryPoints> dxil = std::make_shared<DxilEntryPoints>(entryPoints, blob);

	return dxil;
}

void RenderResources::InitShaderNames()
{
	//rayGenName = L"rayGen";

	//closestHitNames.resize(1);
	//closestHitNames[0] = L"chs";

	//missNames.resize(1);
	//missNames[0] = L"miss";
}

void RenderResources::InitShaderTable()
{
	shaderTable = std::make_shared<ShaderTable>();

	ShaderTable::ShaderInfo info;
	info.heapIndices.push_back(0);
	info.name = rayGenName;
	shaderTable->AddRayGenShader(info);

	info.heapIndices.clear();
	for (auto& miss : missNames)
	{
		info.name = miss;
		shaderTable->AddMissShader(info);
	}

	info.heapIndices.clear();
	for (auto& hit : hitGroupNames)
	{
		info.name = hit;
		shaderTable->AddHitGroupShader(info);
	}

	//ShaderTable::InitStructure init;
	//init.device = device;
	//init.heap = heapCSU;
	//init.pipelineState = pipeline;
	//shaderTable->Create(init);
}

