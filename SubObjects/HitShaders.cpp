#include "HitShaders.h"

HitShaders::HitShaders(const std::wstring& anyHitShaderName,
	const std::wstring& closetHitShader, const std::wstring& hitGroupName)
	: mAnyHit(anyHitShaderName), mClosestHit(closetHitShader), mHitGroup(hitGroupName)
	, mDesc{}, mSubObject{}
{
	mDesc.AnyHitShaderImport = mAnyHit.size() != 0 ? mAnyHit.data() : nullptr;
	mDesc.ClosestHitShaderImport = mClosestHit.data();
	mDesc.HitGroupExport = mHitGroup.data();

	mSubObject.Type = D3D12_STATE_SUBOBJECT_TYPE_HIT_GROUP;
	mSubObject.pDesc = &mDesc;
}

HitShaders::~HitShaders()
{
}

const D3D12_STATE_SUBOBJECT& HitShaders::Get() const
{
	return mSubObject;
}
