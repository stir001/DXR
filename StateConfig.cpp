#include "StateConfig.h"

const D3D12_STATE_SUBOBJECT& StateConfig::Get() const
{
	return mSubobject;
}

void StateConfig::Init()
{
	mConfig.Flags = D3D12_STATE_OBJECT_FLAGS::D3D12_STATE_OBJECT_FLAG_NONE;

	mSubobject.pDesc = &mConfig;
	mSubobject.Type = D3D12_STATE_SUBOBJECT_TYPE_STATE_OBJECT_CONFIG;
}
