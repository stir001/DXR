#pragma once

struct D3D12_RAYTRACING_INSTANCE_DESC;

class InstanceCreator
{
public:
	D3D12_RAYTRACING_INSTANCE_DESC CreateInstanceDesc(long unsigned int gpuVirtualAddress, unsigned int hitGroupIndex);
private:
	unsigned int mInstanceID;
};

