#include "D3DCreateHelper.h"
#include <d3d12.h>
#include <dxgi1_5.h>
#include <Windows.h>

void d3d_create_helper::D3DError(HRESULT hr)
{
	if (SUCCEEDED(hr))
	{
		return;
	}
	LPVOID str;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		nullptr,
		hr,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPWSTR)&str,
		0,
		nullptr);
	OutputDebugString((LPWSTR)str);
	LocalFree(str);
}

MWCptr<ID3D12RootSignature> d3d_create_helper::CreateRootSignature(MWCptr<ID3D12Device5>& device, const D3D12_ROOT_SIGNATURE_DESC& desc)
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

MWCptr<ID3D12Resource> d3d_create_helper::CreateBuffer(MWCptr<ID3D12Device> device, unsigned int bufferSize, D3D12_RESOURCE_FLAGS flag, D3D12_RESOURCE_STATES state, const D3D12_HEAP_PROPERTIES& heapProps)
{
	D3D12_RESOURCE_DESC desc = {};
	desc.Alignment = 0;
	desc.DepthOrArraySize = 1;
	desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	desc.Flags = flag;
	desc.Format = DXGI_FORMAT_UNKNOWN;
	desc.Height = 1;
	desc.Width = bufferSize;
	desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	desc.MipLevels = 1;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;

	MWCptr<ID3D12Resource> resource;
	device->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAG_NONE, &desc, state, nullptr, IID_PPV_ARGS(&resource));

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

MWCptr<IDXGIFactory4> d3d_create_helper::CreateDxgiFactory4()
{
	MWCptr<IDXGIFactory4> dxgiFactory;
	auto hr = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory));
	D3DError(hr);
	return dxgiFactory;
}
