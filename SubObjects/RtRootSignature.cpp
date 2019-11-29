#include "RtRootSignature.h"
#include "D3DCreateHelper.h"

RtRootSignature::RtRootSignature(const MWCptr<ID3D12Device5>& device, const D3D12_ROOT_SIGNATURE_DESC& desc, bool isLocal)
	: mSubObject{}, mRootSignature(nullptr)
{
	Init(device, desc, isLocal);
}

RtRootSignature::RtRootSignature(const MWCptr<ID3D12RootSignature>& rootSignature, bool isLocal)
	: mRootSignature(rootSignature), mInterface(mRootSignature.Get())
{
	mSubObject.pDesc = &mInterface;
	mSubObject.Type = isLocal ? D3D12_STATE_SUBOBJECT_TYPE_LOCAL_ROOT_SIGNATURE : D3D12_STATE_SUBOBJECT_TYPE_GLOBAL_ROOT_SIGNATURE;
}

RtRootSignature::~RtRootSignature()
{
}

void RtRootSignature::Init(const MWCptr<ID3D12Device5>& device, const D3D12_ROOT_SIGNATURE_DESC& desc, bool isLocal)
{
	mRootSignature = d3d_create_helper::CreateRootSignature(device, desc);
	mInterface = mRootSignature.Get();
	mSubObject.pDesc = &mInterface;
	mSubObject.Type = isLocal ? D3D12_STATE_SUBOBJECT_TYPE_LOCAL_ROOT_SIGNATURE : D3D12_STATE_SUBOBJECT_TYPE_GLOBAL_ROOT_SIGNATURE;
}

const D3D12_STATE_SUBOBJECT& RtRootSignature::Get() const
{
	return mSubObject;
}
