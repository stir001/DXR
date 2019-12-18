#include "D3DDescriptorHeap.h"
#include "D3DCreateHelper.h"
#include <d3d12.h>

struct D3DDescriptorHeap::CbvDesc : public D3DDescriptorHeap::ViewDesc
{
	CbvDesc(const D3D12_CONSTANT_BUFFER_VIEW_DESC& cbv) : desc(cbv) {}
	D3D12_CONSTANT_BUFFER_VIEW_DESC desc;
	void CreateView(D3DDescriptorHeap& heap, const D3D12_CPU_DESCRIPTOR_HANDLE& handle);
};

struct D3DDescriptorHeap::SrvDesc : public D3DDescriptorHeap::ViewDesc
{
	SrvDesc(const D3D12_SHADER_RESOURCE_VIEW_DESC& srv, const MWCptr<ID3D12Resource>& resource)
		: desc(srv), resource(resource) {}
	D3D12_SHADER_RESOURCE_VIEW_DESC desc;
	MWCptr<ID3D12Resource> resource;
	void CreateView(D3DDescriptorHeap& heap, const D3D12_CPU_DESCRIPTOR_HANDLE& handle);
};

struct D3DDescriptorHeap::UavDesc : public D3DDescriptorHeap::ViewDesc
{
	UavDesc(const D3D12_UNORDERED_ACCESS_VIEW_DESC& uav, const MWCptr<ID3D12Resource>& resource, const MWCptr<ID3D12Resource>& counter)
		: desc(uav), resource(resource), counter(counter) {}
	D3D12_UNORDERED_ACCESS_VIEW_DESC desc;
	MWCptr<ID3D12Resource> resource;
	MWCptr<ID3D12Resource> counter;
	void CreateView(D3DDescriptorHeap& heap, const D3D12_CPU_DESCRIPTOR_HANDLE& handle);
};

struct D3DDescriptorHeap::RtvDesc : public D3DDescriptorHeap::ViewDesc
{
	RtvDesc(const D3D12_RENDER_TARGET_VIEW_DESC& rtv, const MWCptr<ID3D12Resource>& resource)
		: desc(rtv), resource(resource) {}
	D3D12_RENDER_TARGET_VIEW_DESC desc;
	MWCptr<ID3D12Resource> resource;
	void CreateView(D3DDescriptorHeap& heap, const D3D12_CPU_DESCRIPTOR_HANDLE& handle);
};

struct D3DDescriptorHeap::DSvDesc : public D3DDescriptorHeap::ViewDesc
{
	DSvDesc(const D3D12_DEPTH_STENCIL_VIEW_DESC& dsv, const MWCptr<ID3D12Resource>& resource)
		: desc(dsv), resource(resource) {}
	D3D12_DEPTH_STENCIL_VIEW_DESC desc;
	MWCptr<ID3D12Resource> resource;
	void CreateView(D3DDescriptorHeap& heap, const D3D12_CPU_DESCRIPTOR_HANDLE& handle);
};

struct D3DDescriptorHeap::SmpDesc : public D3DDescriptorHeap::ViewDesc
{
	SmpDesc(const D3D12_SAMPLER_DESC& smp) : desc(smp) {}
	D3D12_SAMPLER_DESC desc;
	void CreateView(D3DDescriptorHeap& heap, const D3D12_CPU_DESCRIPTOR_HANDLE& handle);
};

D3DDescriptorHeap::D3DDescriptorHeap(const MWCptr<ID3D12Device5>& device)
{
	Init(device);
}

D3DDescriptorHeap::~D3DDescriptorHeap()
{
}

void D3DDescriptorHeap::Init(const MWCptr<ID3D12Device5>& device)
{
	mHeap = nullptr;
	mDevice = device;
	//mAvailableIndices.clear();
	mType = D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES;
}

unsigned int D3DDescriptorHeap::AddViewDesc(const D3D12_CONSTANT_BUFFER_VIEW_DESC & cbv)
{
	if (!IsSameHeapType(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV))
	{
		return UINT_MAX;
	}
	
	mDescs.emplace_back(std::make_shared<CbvDesc>(cbv));
	return static_cast<unsigned int>(mDescs.size()) - 1;
}

unsigned int D3DDescriptorHeap::AddViewDesc(const D3D12_SHADER_RESOURCE_VIEW_DESC & srv, const MWCptr<ID3D12Resource>& resource)
{
	if (!IsSameHeapType(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV))
	{
		return UINT_MAX;
	}

	mDescs.emplace_back(std::make_shared<SrvDesc>(srv, resource));
	return static_cast<unsigned int>(mDescs.size()) - 1;
}

unsigned int D3DDescriptorHeap::AddViewDesc(const D3D12_UNORDERED_ACCESS_VIEW_DESC & uav, const MWCptr<ID3D12Resource>& resource, const MWCptr<ID3D12Resource>& counter)
{
	if (!IsSameHeapType(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV))
	{
		return UINT_MAX;
	}

	mDescs.emplace_back(std::make_shared<UavDesc>(uav, resource, counter));
	return static_cast<unsigned int>(mDescs.size()) - 1;
}

unsigned int D3DDescriptorHeap::AddViewDesc(const D3D12_RENDER_TARGET_VIEW_DESC & rtv, const MWCptr<ID3D12Resource>& resource)
{
	if (!IsSameHeapType(D3D12_DESCRIPTOR_HEAP_TYPE_RTV))
	{
		return UINT_MAX;
	}
	
	mDescs.emplace_back(std::make_shared<RtvDesc>(rtv, resource));
	return static_cast<unsigned int>(mDescs.size()) - 1;
}

unsigned int D3DDescriptorHeap::AddViewDesc(const D3D12_DEPTH_STENCIL_VIEW_DESC & dsv, const MWCptr<ID3D12Resource>& resource)
{
	if (!IsSameHeapType(D3D12_DESCRIPTOR_HEAP_TYPE_DSV))
	{
		return UINT_MAX;
	}

	mDescs.emplace_back(std::make_shared<DSvDesc>(dsv, resource));
	return static_cast<unsigned int>(mDescs.size()) - 1;
}

unsigned int D3DDescriptorHeap::AddViewDesc(const D3D12_SAMPLER_DESC & smp)
{
	if (!IsSameHeapType(D3D12_DESCRIPTOR_HEAP_TYPE_RTV))
	{
		return UINT_MAX;
	}

	mDescs.emplace_back(std::make_shared<SmpDesc>(smp));
	return static_cast<unsigned int>(mDescs.size()) - 1;
}

void D3DDescriptorHeap::ReplaceViewDesc(const D3D12_CONSTANT_BUFFER_VIEW_DESC& cbv, unsigned int index)
{
	mDevice->CreateConstantBufferView(&cbv, GetCpuHandle(index));
}

void D3DDescriptorHeap::ReplaceViewDesc(const D3D12_SHADER_RESOURCE_VIEW_DESC& srv, const MWCptr<ID3D12Resource>& resource, unsigned int index)
{
	mDevice->CreateShaderResourceView(resource.Get(), &srv, GetCpuHandle(index));
}

void D3DDescriptorHeap::ReplaceViewDesc(const D3D12_UNORDERED_ACCESS_VIEW_DESC& uav, const MWCptr<ID3D12Resource>& resource, const MWCptr<ID3D12Resource>& counter, unsigned int index)
{
	mDevice->CreateUnorderedAccessView(resource.Get(), counter.Get(), &uav, GetCpuHandle(index));
}

void D3DDescriptorHeap::ReplaceViewDesc(const D3D12_RENDER_TARGET_VIEW_DESC& rtv, const MWCptr<ID3D12Resource>& resource, unsigned int index)
{
	mDevice->CreateRenderTargetView(resource.Get(), &rtv, GetCpuHandle(index));
}

void D3DDescriptorHeap::ReplaceViewDesc(const D3D12_DEPTH_STENCIL_VIEW_DESC& dsv, const MWCptr<ID3D12Resource>& resource, unsigned int index)
{
	mDevice->CreateDepthStencilView(resource.Get(), &dsv, GetCpuHandle(index));
}

void D3DDescriptorHeap::ReplaceViewDesc(const D3D12_SAMPLER_DESC& smp, unsigned int index)
{
	mDevice->CreateSampler(&smp, GetCpuHandle(index));
}

void D3DDescriptorHeap::CreateViews(bool isShaderVisible)
{
	mHeap = d3d_create_helper::CreateDescriptorHeap(mDevice, mType, static_cast<unsigned int>(mDescs.size()), isShaderVisible);

	auto handle = mHeap->GetCPUDescriptorHandleForHeapStart();
	UINT64 size = mDevice->GetDescriptorHandleIncrementSize(mType);
	for (auto& d : mDescs)
	{
		if (d.use_count() != 0)
		{
			d->CreateView(*this, handle);
		}
		handle.ptr += size;
	}
}

D3D12_CPU_DESCRIPTOR_HANDLE D3DDescriptorHeap::GetCpuHandle(unsigned int offset) const
{
	auto handle = mHeap->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += mDevice->GetDescriptorHandleIncrementSize(mType) * static_cast<UINT64>(offset);
	return handle;
}

D3D12_GPU_DESCRIPTOR_HANDLE D3DDescriptorHeap::GetGpuHandle(unsigned int offset) const
{
	auto handle = mHeap->GetGPUDescriptorHandleForHeapStart();
	handle.ptr += mDevice->GetDescriptorHandleIncrementSize(mType) * static_cast<UINT64>(offset);
	return handle;
}

D3D12_DESCRIPTOR_HEAP_TYPE D3DDescriptorHeap::GetHeapType() const
{
	return mType;
}

unsigned int D3DDescriptorHeap::GetHeapIncrementSize() const
{
	return mDevice->GetDescriptorHandleIncrementSize(mType);
}

void D3DDescriptorHeap::RemoveViewDesc(unsigned int index)
{
	if (mDescs.size() < index)
	{
		return;
	}
	mDescs[index].reset();
}

void D3DDescriptorHeap::SetHeap(const MWCptr<ID3D12GraphicsCommandList4>& cmdList)
{
	cmdList->SetDescriptorHeaps(1, mHeap.GetAddressOf());
}

void D3DDescriptorHeap::CreateView(CbvDesc& cbv, const D3D12_CPU_DESCRIPTOR_HANDLE& handle)
{
	mDevice->CreateConstantBufferView(&cbv.desc, handle);
}

void D3DDescriptorHeap::CreateView(SrvDesc& srv, const D3D12_CPU_DESCRIPTOR_HANDLE& handle)
{
	mDevice->CreateShaderResourceView(srv.resource.Get(), &srv.desc, handle);
}

void D3DDescriptorHeap::CreateView(UavDesc& uav, const D3D12_CPU_DESCRIPTOR_HANDLE& handle)
{
	mDevice->CreateUnorderedAccessView(uav.resource.Get(), uav.counter.Get(), &uav.desc, handle);
}

void D3DDescriptorHeap::CreateView(RtvDesc& rtv, const D3D12_CPU_DESCRIPTOR_HANDLE& handle)
{
	mDevice->CreateRenderTargetView(rtv.resource.Get(), &rtv.desc, handle);
}

void D3DDescriptorHeap::CreateView(DSvDesc& dsv, const D3D12_CPU_DESCRIPTOR_HANDLE& handle)
{
	mDevice->CreateDepthStencilView(dsv.resource.Get(), &dsv.desc, handle);
}

void D3DDescriptorHeap::CreateView(SmpDesc& smp, const D3D12_CPU_DESCRIPTOR_HANDLE& handle)
{
	mDevice->CreateSampler(&smp.desc, handle);
}

bool D3DDescriptorHeap::IsSameHeapType(D3D12_DESCRIPTOR_HEAP_TYPE type)
{
	if(mType == type)
	{ 
		return true;
	}
	else if (mType == D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES)
	{
		mType = type;
		return true;
	}

	return false;
}

unsigned int D3DDescriptorHeap::GetIndex() const
{
	unsigned int index = UINT_MAX;
	//for (unsigned int i = 0; i < mAvailableIndices.size(); ++i)
	//{
	//	if (mAvailableIndices[i] == true)
	//	{
	//		index = i;
	//		break;
	//	}
	//}
	return index;
}

void D3DDescriptorHeap::CbvDesc::CreateView(D3DDescriptorHeap& heap, const D3D12_CPU_DESCRIPTOR_HANDLE& handle)
{
	heap.CreateView(*this, handle);
}

void D3DDescriptorHeap::SrvDesc::CreateView(D3DDescriptorHeap& heap, const D3D12_CPU_DESCRIPTOR_HANDLE& handle)
{
	heap.CreateView(*this, handle);
}

void D3DDescriptorHeap::UavDesc::CreateView(D3DDescriptorHeap& heap, const D3D12_CPU_DESCRIPTOR_HANDLE& handle)
{
	heap.CreateView(*this, handle);
}

void D3DDescriptorHeap::RtvDesc::CreateView(D3DDescriptorHeap& heap, const D3D12_CPU_DESCRIPTOR_HANDLE& handle)
{
	heap.CreateView(*this, handle);
}

void D3DDescriptorHeap::DSvDesc::CreateView(D3DDescriptorHeap& heap, const D3D12_CPU_DESCRIPTOR_HANDLE& handle)
{
	heap.CreateView(*this, handle);
}

void D3DDescriptorHeap::SmpDesc::CreateView(D3DDescriptorHeap& heap, const D3D12_CPU_DESCRIPTOR_HANDLE& handle)
{
	heap.CreateView(*this, handle);
}
