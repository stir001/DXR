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
class DxInput;
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
struct Vector4;
struct Vector3;

typedef void* HANDLE;

static const unsigned int SWAP_CHAIN_BUFFER_COUNT = 2U;

class RayTraceRenderer
{
public:
	RayTraceRenderer(HINSTANCE hinst);
	~RayTraceRenderer();

	void Init();
	void BeginFrame();//毎フレーム最初に呼ぶ初期化処理
	void Render();
	void WaitFence();
	void ExcuteCommandList(const MWCptr<ID3D12GraphicsCommandList>& cmdList);
	void SetEvent();
	unsigned int GetCurrentBackBufferIndex() const;
	D3DResource GetRtBuffer(unsigned int index) const;
	const RenderResources::WindowSize& GetWindowSize() const;
	const MWCptr<ID3D12GraphicsCommandList4>& GetCommandList() const;
	const MWCptr<ID3D12Device5>& GetDevice() const;
	std::shared_ptr<DxInput> CreateDxInput() const;
	std::shared_ptr<Camera> GetCamera() const;
	std::vector<std::shared_ptr<GameObject>>& GetGameObjects();
	void CreateViews();
private:
	void InitWindow();
	void ClearRenderTargetView();
	void ResetCommandList();
	void SwapChainPresent();
	void DispatchRays();
	unsigned int LoadColor8Powns(std::vector<std::shared_ptr<GameObject>>& objects, const Vector4& color, unsigned int indexOffset);//indexOffsetにロードした分を追加した数字を返す(+8)
	unsigned int LoadConstantPieces(std::vector<std::shared_ptr<GameObject>>& objects, const Vector4& color, unsigned int indexOffset, bool reverse = false);//indexOffsetにロードした分を追加した数字を返す(+3)
	unsigned int LoadOneSide(std::vector<std::shared_ptr<GameObject>>& objects, const Vector4& color, unsigned int indexOffset);

	RenderResources mResources;
	std::shared_ptr<DrawObjectCreator> mCreator;
	HINSTANCE mHInst;
	HWND mHwnd;
	std::vector<std::shared_ptr<GameObject>> mGameObjects;

	std::shared_ptr<Camera> mCamera;
};

