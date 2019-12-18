#pragma once
#include "SubObjects/SubObject.h"
#include <d3d12.h>
#include <dxcapi.h>


class StateConfig :
	public sub_objects::SubObject
{
public:
	const D3D12_STATE_SUBOBJECT& Get() const ;
private:
	void Init();
	D3D12_STATE_OBJECT_CONFIG mConfig;
	D3D12_STATE_SUBOBJECT mSubobject;
};

