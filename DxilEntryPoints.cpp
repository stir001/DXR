#include <algorithm>
#include "DxilEntryPoints.h"
#include "CharConvert.h"

DxilEntryPoints::DxilEntryPoints(std::vector<std::string>& entryPoints, MWCptr<IDxcBlob> blob) : mDxilLib{}, mLibSubObject{}, mShaderBlob()
{
	SetEntryPoints(entryPoints, blob);
}

DxilEntryPoints::DxilEntryPoints(std::vector<std::wstring>& entryPoints, MWCptr<IDxcBlob> blob) : mDxilLib{}, mLibSubObject{}, mShaderBlob()
{
	SetEntryPoints(entryPoints, blob);
}

DxilEntryPoints::~DxilEntryPoints()
{
}

void DxilEntryPoints::SetEntryPoints(std::vector<std::string>& entryPoints, MWCptr<IDxcBlob> blob)
{
	std::vector<std::wstring> wEntryPoints;
	unsigned int entryPointCount = static_cast<unsigned int>(entryPoints.size());
	for (unsigned int i = 0; i < entryPointCount; ++i)
	{
		ToWChar(wEntryPoints[i], entryPoints[i]);
	}
	SetEntryPoints(wEntryPoints, blob);
}

void DxilEntryPoints::SetEntryPoints(std::vector<std::wstring>& entryPoints, MWCptr<IDxcBlob> blob)
{
	mShaderBlob = blob;
	unsigned int entryPointCount = static_cast<unsigned int>(entryPoints.size());
	mEntryPointNames.resize(entryPointCount);
	std::copy(mEntryPointNames.begin(), mEntryPointNames.end(), entryPoints.begin());
	mExportsDesc.resize(entryPointCount);
	for (unsigned int i = 0; i < entryPointCount; ++i)
	{
		mExportsDesc[i].Name = mEntryPointNames[i].data();
		mExportsDesc[i].Flags = D3D12_EXPORT_FLAGS::D3D12_EXPORT_FLAG_NONE;
		mExportsDesc[i].ExportToRename = nullptr;
	}

	mDxilLib.DXILLibrary.pShaderBytecode = mShaderBlob->GetBufferPointer();
	mDxilLib.DXILLibrary.BytecodeLength = mShaderBlob->GetBufferSize();
	mDxilLib.NumExports = entryPointCount;
	mDxilLib.pExports = mExportsDesc.data();

	mLibSubObject.Type = D3D12_STATE_SUBOBJECT_TYPE::D3D12_STATE_SUBOBJECT_TYPE_DXIL_LIBRARY;
	mLibSubObject.pDesc = &mDxilLib;
}

const D3D12_STATE_SUBOBJECT& DxilEntryPoints::GetDxilLibrary() const
{
	return mLibSubObject;
}
