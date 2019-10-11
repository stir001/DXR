#include "ExportAssociation.h"

ExportAssociation::ExportAssociation(std::vector<std::wstring>& exportNames, const D3D12_STATE_SUBOBJECT& subobjectToAssociation)
{
	Init(exportNames,subobjectToAssociation);
}

ExportAssociation::~ExportAssociation()
{
}

D3D12_STATE_SUBOBJECT ExportAssociation::Get() const
{
	return mSubObject;
}

void ExportAssociation::Init(std::vector<std::wstring>& exportNames, const D3D12_STATE_SUBOBJECT& subobjectToAssociation)
{
	const unsigned int nameCount = static_cast<unsigned int>(exportNames.size());
	mNames.resize(nameCount);
	for (unsigned int i = 0; i < nameCount; i++)
	{
		mNames[i] = &exportNames[i][0];
	}
	mAssociation.NumExports = nameCount;
	mAssociation.pExports = &mNames[0];
	mAssociation.pSubobjectToAssociate = &subobjectToAssociation;

	mSubObject.Type = D3D12_STATE_SUBOBJECT_TYPE_SUBOBJECT_TO_EXPORTS_ASSOCIATION;
	mSubObject.pDesc = &mAssociation;
}
