#include "HitShaders.h"

HitShaders::HitShaders(const std::wstring& anyHitShaderName,
	const std::wstring& closetHitShader, const std::wstring& hitGroupName)
	: mAnyHit(anyHitShaderName), mClosetHit(closetHitShader), mHitGroup(hitGroupName)
	, mDesc{}, mSubObject{}
{
	mDesc.AnyHitShaderImport = mAnyHit.data();
	mDesc.ClosestHitShaderImport = mClosetHit.data();
	mDesc.HitGroupExport = mHitGroup.data();

	mSubObject.Type = D3D12_STATE_SUBOBJECT_TYPE_HIT_GROUP;
	mSubObject.pDesc = &mDesc;
}

HitShaders::~HitShaders()
{
}

D3D12_STATE_SUBOBJECT HitShaders::Get() const
{
	return mSubObject;
}
