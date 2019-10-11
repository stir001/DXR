#pragma once
#include "SubObject.h"
#include <d3d12.h>

class RtPielineConfig :
	public sub_objects::SubObject
{
public:
	RtPielineConfig(unsigned int maxTraceRecursionDepth);
	~RtPielineConfig();

	void Init(unsigned int maxTraceRecursionDepth);
	D3D12_STATE_SUBOBJECT Get() const;
private:
	D3D12_STATE_SUBOBJECT mSubobject;
	D3D12_RAYTRACING_PIPELINE_CONFIG mConfig;
};

