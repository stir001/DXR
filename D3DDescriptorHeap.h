#pragma once
#include "Comptr.h"
#include <vector>
#include <memory>

struct ID3D12GraphicsCommandList4;
struct ID3D12DescriptorHeap;
struct ID3D12Device5;
struct ID3D12Resource;
struct D3D12_CONSTANT_BUFFER_VIEW_DESC;
struct D3D12_SHADER_RESOURCE_VIEW_DESC;
struct D3D12_UNORDERED_ACCESS_VIEW_DESC;
struct D3D12_CPU_DESCRIPTOR_HANDLE;
struct D3D12_RENDER_TARGET_VIEW_DESC;
struct D3D12_DEPTH_STENCIL_VIEW_DESC;
struct D3D12_SAMPLER_DESC;
struct D3D12_GPU_DESCRIPTOR_HANDLE;

enum D3D12_DESCRIPTOR_HEAP_TYPE : int;

class D3DDescriptorHeap
{
public:
	D3DDescriptorHeap(const MWCptr<ID3D12Device5>& device);
	~D3DDescriptorHeap();

	void Init(const MWCptr<ID3D12Device5>& device);

	//Descを追加した後そのIndexを返す(オフセット計算用?)
	[[nodiscard]] unsigned int AddViewDesc(const D3D12_CONSTANT_BUFFER_VIEW_DESC & cbv);
	[[nodiscard]] unsigned int AddViewDesc(const D3D12_SHADER_RESOURCE_VIEW_DESC & srv, const MWCptr<ID3D12Resource>& resource);
	[[nodiscard]] unsigned int AddViewDesc(const D3D12_UNORDERED_ACCESS_VIEW_DESC & uav, const MWCptr<ID3D12Resource>& resource, const MWCptr<ID3D12Resource>& counter);
	[[nodiscard]] unsigned int AddViewDesc(const D3D12_RENDER_TARGET_VIEW_DESC & rtv, const MWCptr<ID3D12Resource>& resource);
	[[nodiscard]] unsigned int AddViewDesc(const D3D12_DEPTH_STENCIL_VIEW_DESC & dsv, const MWCptr<ID3D12Resource>& resource);
	[[nodiscard]] unsigned int AddViewDesc(const D3D12_SAMPLER_DESC & smp);

	void ReplaceViewDesc(const D3D12_CONSTANT_BUFFER_VIEW_DESC& cbv, unsigned int index);
	void ReplaceViewDesc(const D3D12_SHADER_RESOURCE_VIEW_DESC& srv, const MWCptr<ID3D12Resource>& resource, unsigned int index);
	void ReplaceViewDesc(const D3D12_UNORDERED_ACCESS_VIEW_DESC& uav, const MWCptr<ID3D12Resource>& resource, const MWCptr<ID3D12Resource>& counter, unsigned int index);
	void ReplaceViewDesc(const D3D12_RENDER_TARGET_VIEW_DESC& rtv, const MWCptr<ID3D12Resource>& resource, unsigned int index);
	void ReplaceViewDesc(const D3D12_DEPTH_STENCIL_VIEW_DESC& dsv, const MWCptr<ID3D12Resource>& resource, unsigned int index);
	void ReplaceViewDesc(const D3D12_SAMPLER_DESC& smp, unsigned int index);

	void CreateViews(bool isShaderVisible = true);
	D3D12_CPU_DESCRIPTOR_HANDLE GetCpuHandle(unsigned int offset) const;
	D3D12_GPU_DESCRIPTOR_HANDLE GetGpuHandle(unsigned int offset) const;
	D3D12_DESCRIPTOR_HEAP_TYPE GetHeapType() const;
	unsigned int GetHeapIncrementSize() const;

	void RemoveViewDesc(unsigned int index);

	void SetHeap(const MWCptr<ID3D12GraphicsCommandList4> & cmdList);
private:
	struct ViewDesc
	{
		friend D3DDescriptorHeap;
		virtual void CreateView(D3DDescriptorHeap& heap, const D3D12_CPU_DESCRIPTOR_HANDLE& handle)  = 0;
	};

	struct CbvDesc;
	struct SrvDesc;
	struct UavDesc;
	struct RtvDesc;
	struct DSvDesc;
	struct SmpDesc;

	void CreateView(CbvDesc& cbv, const D3D12_CPU_DESCRIPTOR_HANDLE& handle);
	void CreateView(SrvDesc& srv, const D3D12_CPU_DESCRIPTOR_HANDLE& handle);
	void CreateView(UavDesc& uav, const D3D12_CPU_DESCRIPTOR_HANDLE& handle);
	void CreateView(RtvDesc& rtv, const D3D12_CPU_DESCRIPTOR_HANDLE& handle);
	void CreateView(DSvDesc& dsv, const D3D12_CPU_DESCRIPTOR_HANDLE& handle);
	void CreateView(SmpDesc& smp, const D3D12_CPU_DESCRIPTOR_HANDLE& handle);

	bool IsSameHeapType(D3D12_DESCRIPTOR_HEAP_TYPE type);
	unsigned int GetIndex() const;

	MWCptr<ID3D12Device5> mDevice;
	MWCptr<ID3D12DescriptorHeap> mHeap;
	//TODO:こいつ後回し
	//std::vector<bool> mAvailableIndices;
	std::vector<std::shared_ptr<ViewDesc>> mDescs;
	D3D12_DESCRIPTOR_HEAP_TYPE mType;
};

