#include "D3DCreateHelper.h"
#include <d3d12.h>
#include <dxgi1_5.h>
#include <Windows.h>

void d3d_create_helper::D3DError(HRESULT hr)
{
#ifdef _DEBUG
	if (SUCCEEDED(hr))
	{
		return;
	}
	LPVOID str;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		nullptr,
		hr,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPSTR)&str,
		0,
		nullptr);
	OutputDebugString((LPSTR)str);
	LocalFree(str);
#endif // _DEBUG
}

MWCptr<ID3D12RootSignature> d3d_create_helper::CreateRootSignature(const MWCptr<ID3D12Device5>& device, const D3D12_ROOT_SIGNATURE_DESC& desc)
{
	MWCptr<ID3D12RootSignature> rootSignature;
	MWCptr<ID3DBlob> rootBlob;
	MWCptr<ID3DBlob> errorBlob;
	auto hr = D3D12SerializeRootSignature(&desc, D3D_ROOT_SIGNATURE_VERSION_1, &rootBlob, &errorBlob);
	if (FAILED(hr))
	{
		OutputDebugStringA((LPCSTR)errorBlob->GetBufferPointer());
		return nullptr;
	}
	hr = device->CreateRootSignature(0, rootBlob->GetBufferPointer(), rootBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature));
	return rootSignature;
}

MWCptr<ID3D12Resource> d3d_create_helper::CreateBuffer(const MWCptr<ID3D12Device>& device, unsigned int width, D3D12_RESOURCE_FLAGS flag, D3D12_RESOURCE_STATES state, const D3D12_HEAP_PROPERTIES& heapProps)
{
	D3D12_RESOURCE_DESC desc = {};
	desc.Alignment = 0;
	desc.DepthOrArraySize = 1;
	desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	desc.Flags = flag;
	desc.Format = DXGI_FORMAT_UNKNOWN;
	desc.Height = 1;
	desc.Width = width;
	desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	desc.MipLevels = 1;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;

	MWCptr<ID3D12Resource> resource;
	device->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAG_NONE, &desc, state, nullptr, IID_PPV_ARGS(&resource));
	return resource;
}

MWCptr<ID3D12Resource> d3d_create_helper::CreateTexture2D(const MWCptr<ID3D12Device>& device, unsigned int height, unsigned int width, D3D12_TEXTURE_LAYOUT layout, DXGI_FORMAT format, D3D12_RESOURCE_FLAGS flag)
{
	D3D12_RESOURCE_DESC desc = {};
	desc.DepthOrArraySize = 1;
	desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	desc.Format = format;
	desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
	desc.Flags = flag;
	desc.Height = height;
	desc.Width = width;
	desc.MipLevels = 1;
	desc.SampleDesc.Count = 1;

	MWCptr<ID3D12Resource> resource;
	device->CreateCommittedResource(&GetDefaultHeapProps(), D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATE_COMMON, nullptr, IID_PPV_ARGS(&resource));

	return resource;
}

D3D12_HEAP_PROPERTIES d3d_create_helper::GetUploadHeapProps()
{
	return D3D12_HEAP_PROPERTIES{ D3D12_HEAP_TYPE_UPLOAD,
		D3D12_CPU_PAGE_PROPERTY_UNKNOWN,
		D3D12_MEMORY_POOL_UNKNOWN,
		0,
		0 };
}

D3D12_HEAP_PROPERTIES d3d_create_helper::GetDefaultHeapProps()
{
	return D3D12_HEAP_PROPERTIES{ D3D12_HEAP_TYPE_DEFAULT,
	D3D12_CPU_PAGE_PROPERTY_UNKNOWN,
	D3D12_MEMORY_POOL_UNKNOWN,
	0,
	0 };
}

MWCptr<ID3D12Device5> d3d_create_helper::CreateDevice5(const MWCptr<IDXGIFactory>& dxgiFactory)
{
	MWCptr<IDXGIAdapter> hardwareAdapter;
	MWCptr<IDXGIAdapter> adapter;
	MWCptr<ID3D12Device5> device = nullptr;

	{
		D3D_FEATURE_LEVEL levels[] = {
			D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_12_1,
			D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_12_0,
			D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_0,
		};

		for (UINT i = 0; device == nullptr && DXGI_ERROR_NOT_FOUND != dxgiFactory->EnumAdapters(i, adapter.GetAddressOf()); ++i)
		{
			DXGI_ADAPTER_DESC adaptorDesc = {};
			adapter->GetDesc(&adaptorDesc);

			for (auto level : levels)
			{
				if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), level, IID_PPV_ARGS(device.GetAddressOf()))))
				{
					D3D12_FEATURE_DATA_D3D12_OPTIONS5 featureOpation;
					auto hr = device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS5, &featureOpation, sizeof(D3D12_FEATURE_DATA_D3D12_OPTIONS5));
					if (SUCCEEDED(hr) && featureOpation.RaytracingTier != D3D12_RAYTRACING_TIER_NOT_SUPPORTED)
					{
						break;
					}
					else
					{
						device = nullptr;
					}
				}
			}
		}
	}
	return device;
}

MWCptr<ID3D12GraphicsCommandList4> d3d_create_helper::CreateGraphicsCommandList4(const MWCptr<ID3D12Device>& device, MWCptr<ID3D12CommandAllocator>& allocator, D3D12_COMMAND_LIST_TYPE type)
{
	MWCptr<ID3D12GraphicsCommandList4> cmdList = nullptr;
	auto hr = device->CreateCommandList(0, type, allocator.Get(), nullptr, IID_PPV_ARGS(&cmdList));
	D3DError(hr);
	return cmdList;
}

MWCptr<ID3D12CommandAllocator> d3d_create_helper::CreateCommandAllocator(const MWCptr<ID3D12Device>& device, D3D12_COMMAND_LIST_TYPE type)
{
	MWCptr<ID3D12CommandAllocator> allocator = nullptr;
	auto hr = device->CreateCommandAllocator(type, IID_PPV_ARGS(&allocator));
	D3DError(hr);
	return allocator;
}

MWCptr<ID3D12CommandQueue> d3d_create_helper::CreateCommandQueue(const MWCptr<ID3D12Device>& device, D3D12_COMMAND_LIST_TYPE type)
{
	MWCptr<ID3D12CommandQueue> queue = nullptr;
	D3D12_COMMAND_QUEUE_DESC desc = {};
	desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	desc.NodeMask = 0;
	desc.Priority = 0;
	desc.Type = type;
	D3DError(device->CreateCommandQueue(&desc, IID_PPV_ARGS(&queue)));
	return queue;
}

MWCptr<IDXGIFactory4> d3d_create_helper::CreateDxgiFactory4()
{
	MWCptr<IDXGIFactory4> dxgiFactory;
	auto hr = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory));
	D3DError(hr);
	return dxgiFactory;
}

MWCptr<IDXGISwapChain3> d3d_create_helper::CreateSwapChain(const MWCptr<IDXGIFactory4>& factory, MWCptr<ID3D12CommandQueue>& cmdQueue, const HWND& hwnd, unsigned int width, unsigned int height, DXGI_FORMAT format)
{
	MWCptr<IDXGISwapChain1> swapChain = nullptr;

	DXGI_SWAP_CHAIN_DESC1 desc = {};
	desc.BufferCount = 2;
	desc.Width = width;
	desc.Height = height;
	desc.Format = format;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	desc.SampleDesc.Count = 1;

	factory->CreateSwapChainForHwnd(cmdQueue.Get(), hwnd, &desc, nullptr, nullptr, &swapChain);
	MWCptr<IDXGISwapChain3> swapChain3 = nullptr;
	swapChain->QueryInterface<IDXGISwapChain3>(&swapChain3);
	return swapChain3;
}

MWCptr<ID3D12Fence> d3d_create_helper::CreateFence(const MWCptr<ID3D12Device>& device)
{
	MWCptr<ID3D12Fence> fence = nullptr;
	D3DError(device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence)));
	return fence;
}

unsigned int d3d_create_helper::Alignment(unsigned int alignment, unsigned int val)
{
	return ((alignment + val - 1) / alignment) * alignment;
}


D3D12_CONSTANT_BUFFER_VIEW_DESC d3d_create_helper::CreateConstantBufferView(D3D12_GPU_VIRTUAL_ADDRESS address, unsigned int bufferSize)
{
	D3D12_CONSTANT_BUFFER_VIEW_DESC cbv = {};
	cbv.BufferLocation = address;
	cbv.SizeInBytes = bufferSize;
	return cbv;
}

D3D12_SHADER_RESOURCE_VIEW_DESC d3d_create_helper::CreateSRVTexture2D(DXGI_FORMAT format)
{
	D3D12_SHADER_RESOURCE_VIEW_DESC srv = {};
	srv.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srv.Format = format;
	srv.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srv.Texture2D.MipLevels = 1U;
	srv.Texture2D.PlaneSlice = 1U;
	srv.Texture2D.MostDetailedMip = 0U;
	srv.Texture2D.ResourceMinLODClamp = 1.0f;

	return srv;
}

D3D12_SHADER_RESOURCE_VIEW_DESC d3d_create_helper::CreateSRVAS(const MWCptr<ID3D12Resource>& resource)
{
	D3D12_SHADER_RESOURCE_VIEW_DESC desc = {};
	desc.ViewDimension = D3D12_SRV_DIMENSION_RAYTRACING_ACCELERATION_STRUCTURE;
	desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	desc.RaytracingAccelerationStructure.Location = resource->GetGPUVirtualAddress();
	return desc;
}

D3D12_UNORDERED_ACCESS_VIEW_DESC d3d_create_helper::CreateUAVTexture2D()
{
	D3D12_UNORDERED_ACCESS_VIEW_DESC uav = {};
	uav.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2D;
	//uav.Format = DXGI_FORMAT_UNKNOWN;
	//uav.Texture2D.MipSlice = 0U;
	//uav.Texture2D.PlaneSlice = 0U;
	return uav;
}

D3D12_DEPTH_STENCIL_VIEW_DESC d3d_create_helper::CreateDepthStencilView(DXGI_FORMAT format)
{
	D3D12_DEPTH_STENCIL_VIEW_DESC desc = {};
	desc.Flags = D3D12_DSV_FLAG_NONE;
	desc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	desc.Format = format;
	desc.Texture2D.MipSlice = 1U;

	return desc;
}

D3D12_RENDER_TARGET_VIEW_DESC d3d_create_helper::CreateRenderTargetViewDesc(DXGI_FORMAT format)
{
	D3D12_RENDER_TARGET_VIEW_DESC desc = {};
	desc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
	desc.Format = format;
	desc.Texture2D.MipSlice = 0U;
	desc.Texture2D.PlaneSlice = 0U;

	return desc;
}

MWCptr<ID3D12DescriptorHeap> d3d_create_helper::CreateDescriptorHeap(const MWCptr<ID3D12Device5>& device, D3D12_DESCRIPTOR_HEAP_TYPE type, unsigned int descNum, bool isShaderVisible)
{
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.NodeMask = 0;
	desc.Flags = isShaderVisible ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	desc.Type = type;
	desc.NumDescriptors = descNum;

	MWCptr<ID3D12DescriptorHeap> heap;

	device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&heap));
	return heap;
}

D3D12_RAYTRACING_INSTANCE_DESC d3d_create_helper::CreateRaytracingInstanceDesc(unsigned int instantceID, unsigned int hitGroupIndex, const D3D12_GPU_VIRTUAL_ADDRESS& handleBLAS)
{
	D3D12_RAYTRACING_INSTANCE_DESC desc = {};
	desc.AccelerationStructure = handleBLAS;
	desc.InstanceContributionToHitGroupIndex = hitGroupIndex;
	desc.InstanceID = instantceID;
	desc.InstanceMask = 0xff;
	desc.Flags = D3D12_RAYTRACING_INSTANCE_FLAG_FORCE_OPAQUE;
	return desc;
}
