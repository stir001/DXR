#include "RtPielineConfig.h"

RtPielineConfig::RtPielineConfig(unsigned int maxTraceRecursionDepth) 
	: mSubobject{}, mConfig{}
{
	Init(maxTraceRecursionDepth);
}

RtPielineConfig::~RtPielineConfig()
{
}

void RtPielineConfig::Init(unsigned int maxTraceRecursionDepth)
{
	mConfig.MaxTraceRecursionDepth = maxTraceRecursionDepth;
	mSubobject.Type = D3D12_STATE_SUBOBJECT_TYPE_RAYTRACING_PIPELINE_CONFIG;
	mSubobject.pDesc = &mConfig;
}

D3D12_STATE_SUBOBJECT RtPielineConfig::Get() const
{
	return mSubobject;
}
