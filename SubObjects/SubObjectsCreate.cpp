#include "SubObjectsCreate.h"
#include "ExportAssociation.h"
#include "HitShaders.h"
#include "ShaderConfig.h"
#include "RtPipelineConfig.h"
#include "RtRootSignature.h"
#include "StateConfig.h"

using namespace sub_objects;

std::shared_ptr<SubObject> sub_objects::CreateHitGroup(const std::wstring& ahsName, const std::wstring& chsName, const std::wstring& hitGroupName)
{
	std::shared_ptr<SubObject> subObject = std::make_shared<HitShaders>(ahsName, chsName, hitGroupName);
	return subObject;
}

std::shared_ptr<SubObject> sub_objects::CreateExportAssociation(const std::vector<std::wstring>& exportNames, unsigned int subObjectIndex)
{
	std::shared_ptr<SubObject> subObject = std::make_shared<ExportAssociation>(exportNames, subObjectIndex);
	return subObject;
}

std::shared_ptr<SubObject> sub_objects::CreateSubObjectRootSignature(const MWCptr<ID3D12Device5>& device, const D3D12_ROOT_SIGNATURE_DESC& desc, bool isLocal)
{
	std::shared_ptr<SubObject> subObject = std::make_shared<RtRootSignature>(device, desc, isLocal);
	return subObject;
}

std::shared_ptr<SubObject> sub_objects::CreateSubObjectRootSignature(const MWCptr<ID3D12RootSignature>& rootSignature, bool isLocal)
{
	std::shared_ptr<SubObject> subObject = std::make_shared<RtRootSignature>(rootSignature, isLocal);
	return subObject;
}

std::shared_ptr<SubObject> sub_objects::CreateShaderConfig(unsigned int maxAttributeSize, unsigned int maxPayloadSize)
{
	std::shared_ptr<SubObject> subObject = std::make_shared<ShaderConfig>(maxAttributeSize, maxPayloadSize);
	return subObject;
}

std::shared_ptr<SubObject> sub_objects::CreatePipelineConfig(unsigned int maxTraceRecursionDepth)
{
	std::shared_ptr<SubObject> subObject = std::make_shared<RtPipelineConfig>(maxTraceRecursionDepth);
	return subObject;
}

std::shared_ptr<SubObject> sub_objects::CreateStateConfig()
{
	std::shared_ptr<SubObject> object = std::make_shared<StateConfig>();
	return object;
}
