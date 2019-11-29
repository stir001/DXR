#include "RtPipelineConfig.h"

RtPipelineConfig::RtPipelineConfig(unsigned int maxTraceRecursionDepth) 
	: mSubobject{}, mConfig{}
{
	Init(maxTraceRecursionDepth);
}

RtPipelineConfig::~RtPipelineConfig()
{
}

void RtPipelineConfig::Init(unsigned int maxTraceRecursionDepth)
{
	mConfig.MaxTraceRecursionDepth = maxTraceRecursionDepth;
	mSubobject.Type = D3D12_STATE_SUBOBJECT_TYPE_RAYTRACING_PIPELINE_CONFIG;
	mSubobject.pDesc = &mConfig;
}

const D3D12_STATE_SUBOBJECT& RtPipelineConfig::Get() const
{
	return mSubobject;
}
