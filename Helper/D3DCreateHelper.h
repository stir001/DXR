#pragma once
#include "Comptr.h"

struct ID3D12RootSignature;
struct ID3D12Device5;
struct ID3D12Device;
struct D3D12_ROOT_SIGNATURE_DESC;
struct ID3D12Resource;
struct D3D12_HEAP_PROPERTIES;
struct IDXGIFactory;
struct ID3D12GraphicsCommandList4;
struct ID3D12CommandAllocator;
struct IDXGIFactory4;
enum D3D12_RESOURCE_FLAGS : int;
enum D3D12_RESOURCE_STATES : int;
enum D3D12_COMMAND_LIST_TYPE : int;

namespace d3d_create_helper
{
	void D3DError(HRESULT hr);
	MWCptr<ID3D12RootSignature> CreateRootSignature(MWCptr<ID3D12Device5>& device, const D3D12_ROOT_SIGNATURE_DESC& desc);
	MWCptr<ID3D12Resource> CreateBuffer(MWCptr<ID3D12Device> device, unsigned int bufferSize, D3D12_RESOURCE_FLAGS flag, D3D12_RESOURCE_STATES state, const D3D12_HEAP_PROPERTIES& heapProps);
	D3D12_HEAP_PROPERTIES GetUploadHeapProps();
	D3D12_HEAP_PROPERTIES GetDefaultHeapProps();
	MWCptr<ID3D12Device5> CreateDevice5(const MWCptr<IDXGIFactory>& factory);
	MWCptr<ID3D12GraphicsCommandList4> CreateGraphicsCommandList4(const MWCptr<ID3D12Device>& device, MWCptr<ID3D12CommandAllocator>& allocator, D3D12_COMMAND_LIST_TYPE type);
	MWCptr<ID3D12CommandAllocator> CreateCommandAllocator(const MWCptr<ID3D12Device>& device, D3D12_COMMAND_LIST_TYPE type);
	MWCptr<IDXGIFactory4> CreateDxgiFactory4();
}