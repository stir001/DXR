#pragma once
#include "SubObject.h"
#include <d3d12.h>

class RtPipelineConfig :
	public sub_objects::SubObject
{
public:
	RtPipelineConfig(unsigned int maxTraceRecursionDepth);
	~RtPipelineConfig();

	void Init(unsigned int maxTraceRecursionDepth);
	const D3D12_STATE_SUBOBJECT& Get() const;
private:
	D3D12_STATE_SUBOBJECT mSubobject;
	D3D12_RAYTRACING_PIPELINE_CONFIG mConfig;
};

