#pragma once
#include "Comptr.h"
#include <vector>
#include <string>
#include <memory>

struct ID3D12Resource;
struct ID3D12Device5;
struct ID3D12StateObject;
struct ID3D12DescriptorHeap;
class D3DDescriptorHeap;
class RtPipelineState;

typedef UINT64 D3D12_GPU_VIRTUAL_ADDRESS;

class ShaderTable
{
public:
	struct InitStructure
	{
		MWCptr<ID3D12Device5> device;
		std::shared_ptr<RtPipelineState> pipelineState;
		std::shared_ptr<D3DDescriptorHeap> heap;//shaderに関連付けられたheap.ひとまとめにしておく
	};

	struct ShaderInfo
	{
		std::wstring name;
		std::vector<unsigned int> heapIndices;
	};

	ShaderTable();
	~ShaderTable();

	unsigned int AddHitGroupShader(const ShaderInfo& info);
	unsigned int AddRayGenShader(const ShaderInfo& info);
	unsigned int AddMissShader(const ShaderInfo& info);
	void Create(const InitStructure& initData);
	unsigned long long GetShaderTableEntrySize() const;
	D3D12_GPU_VIRTUAL_ADDRESS GetRayGenTableAddress() const;
	unsigned int GetRayGenNum() const;
	D3D12_GPU_VIRTUAL_ADDRESS GetMissTableAddress() const;
	unsigned int GetMissNum() const;
	D3D12_GPU_VIRTUAL_ADDRESS GetHitGroupTableAddress() const;
	unsigned int GetHitGroupNum() const;
private:
	unsigned int GetMaxHeapCount() const;
	void CreateShaderTable(unsigned int maxHeapCount, const MWCptr<ID3D12Device5>& device);
	void WriteShaderTable(const std::shared_ptr<RtPipelineState>& pipelineState, const std::shared_ptr<D3DDescriptorHeap>& heap);

	std::vector<ShaderInfo> mRayGenShaders;
	std::vector<ShaderInfo> mMissShaders;
	std::vector<ShaderInfo> mHitGroupShaders;
	
	unsigned long long mEntrySize;
	MWCptr<ID3D12Resource> mShaderTable;
};

