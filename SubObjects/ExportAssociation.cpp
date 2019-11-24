#include "ExportAssociation.h"
#include "PipelineState/RtPipelineState.h"

ExportAssociation::ExportAssociation(const std::vector<std::wstring>& exportNames, unsigned int subOjectIndex)
{
	Init(exportNames,subOjectIndex);
}

ExportAssociation::~ExportAssociation()
{
}

const D3D12_STATE_SUBOBJECT& ExportAssociation::Get() const
{
	return mSubObject;
}

void ExportAssociation::Init(const std::vector<std::wstring>& exportNames, unsigned int subOjectIndex)
{
	mAssociateIndex = subOjectIndex;

	const unsigned int nameCount = static_cast<unsigned int>(exportNames.size());
	mNames.resize(nameCount);
	for (unsigned int i = 0; i < nameCount; i++)
	{
		mNames[i] = exportNames[i].data();
	}
	mAssociation.NumExports = nameCount;
	mAssociation.pExports = &mNames[0];
	//mAssociation.pSubobjectToAssociate = &subobjectToAssociation;

	mSubObject.Type = D3D12_STATE_SUBOBJECT_TYPE_SUBOBJECT_TO_EXPORTS_ASSOCIATION;
	mSubObject.pDesc = &mAssociation;
}

void ExportAssociation::Create(RtPipelineState& pipelineState)
{
	SetSubObject(pipelineState);
}

void ExportAssociation::SetSubObject(RtPipelineState& pipelineState)
{
	mAssociation.pSubobjectToAssociate = &pipelineState.GetSubObject(mAssociateIndex);
}
