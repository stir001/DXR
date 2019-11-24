#pragma once
#include "SubObject.h"
#include <string>
#include <d3d12.h>

class HitShaders :
	public sub_objects::SubObject
{
public:
	HitShaders(const std::wstring& anyHitShaderName, const std::wstring& closetHitShader, const std::wstring& hitGroupName);
	~HitShaders();

	const D3D12_STATE_SUBOBJECT& Get() const;
private:
	std::wstring mAnyHit;
	std::wstring mClosestHit;
	std::wstring mHitGroup;
	D3D12_HIT_GROUP_DESC mDesc;
	D3D12_STATE_SUBOBJECT mSubObject;
};

