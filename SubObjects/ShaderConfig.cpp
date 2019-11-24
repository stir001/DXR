#include "ShaderConfig.h"

ShaderConfig::ShaderConfig(const unsigned int maxAttributeByte, const unsigned int maxPayloadByte)
	: mConfig{}, mSubObject{}
{
	Init(maxAttributeByte, maxPayloadByte);
}

ShaderConfig::~ShaderConfig()
{
}

void ShaderConfig::Init(const unsigned int maxAttributeByte, const unsigned int maxPayloadByte)
{
	mConfig.MaxAttributeSizeInBytes = maxAttributeByte;
	mConfig.MaxPayloadSizeInBytes = maxPayloadByte;

	mSubObject.Type = D3D12_STATE_SUBOBJECT_TYPE_RAYTRACING_SHADER_CONFIG;
	mSubObject.pDesc = &mConfig;
}

const D3D12_STATE_SUBOBJECT& ShaderConfig::Get() const
{
	return mSubObject;
}
