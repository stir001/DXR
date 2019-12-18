#pragma once
#include "Comptr.h"
#include "RenderResources.h"
#include <memory>
#include <vector>
#include <Windows.h>

class D3DDescriptorHeap;
class RtPipelineState;
class ShaderTable;
class DrawObjectCreator;
class GameObject;
class Camera;
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
struct D3D12_CPU_DESCRIPTOR_HANDLE;
struct D3DResource;

typedef void* HANDLE;

static const unsigned int SWAP_CHAIN_BUFFER_COUNT = 2U;

class RayTraceRenderer
{
public:
	RayTraceRenderer(HINSTANCE hinst);
	~RayTraceRenderer();

	void Init();
	void BeginFrame();//ñàÉtÉåÅ[ÉÄç≈èâÇ…åƒÇ‘èâä˙âªèàóù
	void Render();
	void WaitFence();
	void ExcuteCommandList(const MWCptr<ID3D12GraphicsCommandList>& cmdList);
	void SetEvent();
	unsigned int GetCurrentBackBufferIndex() const;
	D3DResource GetRtBuffer(unsigned int index) const;
	const RenderResources::WindowSize& GetWindowSize() const;
	const MWCptr<ID3D12GraphicsCommandList4>& GetCommandList() const;
	const MWCptr<ID3D12Device5>& GetDevice() const;
	//std::shared_ptr<DrawObjectCreator> GetDrawObjectCreator() const;
	void CreateViews();
private:
	void InitWindow();
	void ClearRenderTargetView();
	void ResetCommandList();
	void SwapChainPresent();
	void DispatchRays();

	RenderResources mResources;
	std::shared_ptr<DrawObjectCreator> mCreator;
	HINSTANCE mHInst;
	HWND mHwnd;
	std::vector<std::shared_ptr<GameObject>> mTestObjects;

	std::shared_ptr<Camera> mCamera;
};

