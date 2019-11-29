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
struct D3D12_CONSTANT_BUFFER_VIEW_DESC;
struct D3D12_SHADER_RESOURCE_VIEW_DESC;
struct D3D12_UNORDERED_ACCESS_VIEW_DESC;
struct D3D12_DEPTH_STENCIL_VIEW_DESC;
struct D3D12_RENDER_TARGET_VIEW_DESC;
struct ID3D12DescriptorHeap;
struct IDXGISwapChain3;
struct ID3D12Fence;
struct ID3D12CommandQueue;
struct D3D12_RAYTRACING_INSTANCE_DESC;
enum D3D12_RESOURCE_FLAGS : int;
enum D3D12_RESOURCE_STATES : int;
enum D3D12_COMMAND_LIST_TYPE : int;
enum D3D12_TEXTURE_LAYOUT : int;
enum DXGI_FORMAT : int;
enum D3D12_RESOURCE_FLAGS : int;
enum D3D12_DESCRIPTOR_HEAP_TYPE : int;

typedef UINT64 D3D12_GPU_VIRTUAL_ADDRESS;

namespace d3d_create_helper
{
	void D3DError(HRESULT hr);
	MWCptr<ID3D12RootSignature> CreateRootSignature(const MWCptr<ID3D12Device5>& device, const D3D12_ROOT_SIGNATURE_DESC& desc);
	MWCptr<ID3D12Resource> CreateBuffer(const MWCptr<ID3D12Device>& device, unsigned int width, D3D12_RESOURCE_FLAGS flag, D3D12_RESOURCE_STATES state, const D3D12_HEAP_PROPERTIES& heapProps);
	MWCptr<ID3D12Resource> CreateTexture2D(const MWCptr<ID3D12Device>& device, unsigned int height, unsigned int width, D3D12_TEXTURE_LAYOUT layout, DXGI_FORMAT format, D3D12_RESOURCE_FLAGS flag);
	D3D12_HEAP_PROPERTIES GetUploadHeapProps();
	D3D12_HEAP_PROPERTIES GetDefaultHeapProps();
	MWCptr<ID3D12Device5> CreateDevice5(const MWCptr<IDXGIFactory>& factory);
	MWCptr<ID3D12GraphicsCommandList4> CreateGraphicsCommandList4(const MWCptr<ID3D12Device>& device, MWCptr<ID3D12CommandAllocator>& allocator, D3D12_COMMAND_LIST_TYPE type);
	MWCptr<ID3D12CommandAllocator> CreateCommandAllocator(const MWCptr<ID3D12Device>& device, D3D12_COMMAND_LIST_TYPE type);
	MWCptr<ID3D12CommandQueue> CreateCommandQueue(const MWCptr<ID3D12Device>& device, D3D12_COMMAND_LIST_TYPE type);
	MWCptr<IDXGIFactory4> CreateDxgiFactory4();
	MWCptr<IDXGISwapChain3> CreateSwapChain(const MWCptr<IDXGIFactory4>& factory, MWCptr<ID3D12CommandQueue>& cmdQueue, const HWND& hwnd, unsigned int width, unsigned int height, DXGI_FORMAT format);
	MWCptr<ID3D12Fence> CreateFence(const MWCptr<ID3D12Device>& device);
	unsigned int Alignment(unsigned int alignment, unsigned int val);
	D3D12_CONSTANT_BUFFER_VIEW_DESC CreateConstantBufferView(D3D12_GPU_VIRTUAL_ADDRESS address, unsigned int bufferSize);
	D3D12_SHADER_RESOURCE_VIEW_DESC CreateSRVTexture2D(DXGI_FORMAT format);
	D3D12_SHADER_RESOURCE_VIEW_DESC CreateSRVAS(const MWCptr<ID3D12Resource>& reosurce);
	D3D12_UNORDERED_ACCESS_VIEW_DESC CreateUAVTexture2D();
	D3D12_DEPTH_STENCIL_VIEW_DESC CreateDepthStencilView(DXGI_FORMAT format);
	D3D12_RENDER_TARGET_VIEW_DESC CreateRenderTargetViewDesc(DXGI_FORMAT format);//texture2d
	MWCptr<ID3D12DescriptorHeap> CreateDescriptorHeap(const MWCptr<ID3D12Device5>& device, D3D12_DESCRIPTOR_HEAP_TYPE type, unsigned int descNum, bool isShaderVisible);
	D3D12_RAYTRACING_INSTANCE_DESC CreateRaytracingInstanceDesc(unsigned int instantceID, unsigned int hitGroupIndex, const D3D12_GPU_VIRTUAL_ADDRESS& handleBLAS);
}