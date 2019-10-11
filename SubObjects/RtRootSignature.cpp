#include "RtRootSignature.h"
#include "D3DCreateHelper.h"

RtRootSignature::RtRootSignature(MWCptr<ID3D12Device5>& device, const D3D12_ROOT_SIGNATURE_DESC& desc, bool isLocal)
	: mSubObject{}, mRootSignature(nullptr)
{
	Init(device, desc, isLocal);
}

RtRootSignature::~RtRootSignature()
{
}

void RtRootSignature::Init(MWCptr<ID3D12Device5>& device, const D3D12_ROOT_SIGNATURE_DESC& desc, bool isLocal)
{
	mRootSignature = d3d_create_helper::CreateRootSignature(device, desc);
	mSubObject.pDesc = mRootSignature.Get();
	mSubObject.Type = isLocal ? D3D12_STATE_SUBOBJECT_TYPE_LOCAL_ROOT_SIGNATURE : D3D12_STATE_SUBOBJECT_TYPE_GLOBAL_ROOT_SIGNATURE;
}

D3D12_STATE_SUBOBJECT RtRootSignature::Get() const
{
	return mSubObject;
}
