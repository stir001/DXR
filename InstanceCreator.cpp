#include "InstanceCreator.h"
#include <d3d12.h>

D3D12_RAYTRACING_INSTANCE_DESC InstanceCreator::CreateInstanceDesc(long unsigned int gpuVirtualAddress, unsigned int hitGroupIndex)
{
	const float identity[3][4] = {
		{1.0f,0.0f,0.0f,0.0f},
		{0.0f,1.0f,0.0f,0.0f},
		{0.0f,0.0f,1.0f,0.0f}
	};

	D3D12_RAYTRACING_INSTANCE_DESC desc;
	desc.InstanceID = 0;
	desc.InstanceContributionToHitGroupIndex = hitGroupIndex;
	memcpy(desc.Transform, identity, sizeof(float) * 3 * 4);
	desc.AccelerationStructure = gpuVirtualAddress;
	desc.InstanceMask = 0xff;
	return desc;
}
