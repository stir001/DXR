#include "DxilLibrarySubobjects.h"

DxilLibrarySubobjects::DxilLibrarySubobjects(const MWCptr<IDxcBlob>& blob) : mShaderBlob(blob), mDxilLib{}, mSubObject{}
{
	Init();
}

const D3D12_STATE_SUBOBJECT& DxilLibrarySubobjects::Get() const
{
	return mSubObject;
}

void DxilLibrarySubobjects::Init()
{
	mExportNames.emplace_back(L"rayGen");
	mExportNames.emplace_back(L"miss");
	mExportNames.emplace_back(L"chessChs");

	mExportNames.emplace_back(L"GlobalRootSig");
	mExportNames.emplace_back(L"RayGenRootSig");
	mExportNames.emplace_back(L"NormalRootSig");
	mExportNames.emplace_back(L"EmptyRootSig");
	mExportNames.emplace_back(L"NormalHitGroup");
	mExportNames.emplace_back(L"RayGenAssociation");
	mExportNames.emplace_back(L"MissAssociation");
	mExportNames.emplace_back(L"ClosestHitAssociation");
	mExportNames.emplace_back(L"ShaderConfig");
	mExportNames.emplace_back(L"PipelineConfig");

	mDxilLib.DXILLibrary.BytecodeLength = mShaderBlob->GetBufferSize();
	mDxilLib.DXILLibrary.pShaderBytecode = mShaderBlob->GetBufferPointer();

	mExportsDesc.resize(mExportNames.size());
	for (unsigned int i = 0; i < mExportsDesc.size(); ++i)
	{
		mExportsDesc[i].Name = mExportNames[i].data();
		mExportsDesc[i].Flags = D3D12_EXPORT_FLAGS::D3D12_EXPORT_FLAG_NONE;
		mExportsDesc[i].ExportToRename = nullptr;
	}

	mDxilLib.NumExports = mExportsDesc.size();
	mDxilLib.pExports = mExportsDesc.data();

	mSubObject.pDesc = &mDxilLib;
	mSubObject.Type = D3D12_STATE_SUBOBJECT_TYPE_DXIL_LIBRARY;
}
