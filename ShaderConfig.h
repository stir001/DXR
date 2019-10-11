#pragma once
#include "SubObject.h"
#include <d3d12.h>
class ShaderConfig :
	public sub_objects::SubObject
{
public:
	ShaderConfig(const unsigned int maxAttributeByte, const unsigned int maxPayloadByte);
	~ShaderConfig();
	void Init(const unsigned int maxAttributeByte, const unsigned int maxPayloadByte);
	D3D12_STATE_SUBOBJECT Get() const;
private:
	D3D12_STATE_SUBOBJECT mSubObject;
	D3D12_RAYTRACING_SHADER_CONFIG mConfig;
};

