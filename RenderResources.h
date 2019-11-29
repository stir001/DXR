#pragma once
#include "Comptr.h"
#include "D3DResource.h"
#include <memory>
#include <vector>
#include <string>

class D3DDescriptorHeap;
class RtPipelineState;
class DxilEntryPoints;
class ShaderTable;
namespace sub_objects { class SubObject; }
struct ID3D12Device5;
struct ID3D12DescriptorHeap;
struct ID3D12CommandList;
struct ID3D12GraphicsCommandList4;
struct ID3D12GraphicsCommandList;
struct ID3D12CommandAllocator;
struct ID3D12CommandQueue;
struct IDXGIFactory;
struct IDXGIFactory4;
struct IDXGISwapChain3;
struct ID3D12Fence;
struct ID3D12Resource;
struct ID3D12RootSignature;
struct D3D12_CPU_DESCRIPTOR_HANDLE;

struct RenderResources
{
public:
	RenderResources();

	void Init(HWND hwnd, unsigned int wndWidth, unsigned int wndHeight);

	struct WindowSize
	{
		unsigned int width;
		unsigned int height;
	};

	MWCptr<ID3D12Device5> device;
	std::shared_ptr<D3DDescriptorHeap> heapCSU;
	std::unique_ptr<D3DDescriptorHeap> heapRtv;
	MWCptr<ID3D12GraphicsCommandList4> cmdList;
	MWCptr<ID3D12CommandAllocator> cmdAllocator;
	MWCptr<ID3D12CommandQueue> cmdQueue;
	MWCptr<IDXGISwapChain3> swapChain;
	MWCptr<IDXGIFactory4> factory;
	MWCptr<ID3D12Fence> fence;
	HANDLE fenceEvent;
	unsigned int fenceValue;
	D3DResource rtvBuffers[2];
	D3DResource outputResource;
	std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> rtvHandles;
	WindowSize size;//window‚Ì‘å‚«‚³
	HWND hwnd;
	std::shared_ptr<sub_objects::SubObject> rayGenRootSig;
	std::shared_ptr<sub_objects::SubObject> missRootSig;
	std::shared_ptr<sub_objects::SubObject> closestHitRootSig;
	MWCptr<ID3D12RootSignature> globalRootSig;
	std::shared_ptr<RtPipelineState> pipeline;
	std::shared_ptr<ShaderTable> shaderTable;
	std::wstring rayGenName; 
	std::vector<std::wstring> closestHitNames;
	std::vector<std::wstring> anyHitNames;
	std::vector<std::wstring> hitGroupNames;
	std::vector<std::wstring> missNames;

private:
	void InitFactory4();
	void InitDevice();
	void InitGraphicsCommandList4();
	void InitCommandQueue();
	void InitSwapChain(HWND& hwnd, unsigned int width, unsigned int height);
	void InitFence();
	void InitFenceEvent();
	void InitRenderTarget();
	void InitDescriptorHeaps();
	void InitOutputResource();

	void InitRayGenRootSig();
	void InitMissRootSig();
	void InitClosestHitRootSig();
	void InitGlobalRootSig();
	void InitPipelineState();
	std::shared_ptr<DxilEntryPoints> CompileShader();
	void InitShaderNames();
	void InitShaderTable();
};

