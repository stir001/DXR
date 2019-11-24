#pragma once
#include <vector>
#include <string>
#include <d3d12.h>
#include <dxcapi.h>
#include "Comptr.h"
#include "SubObjects/SubObject.h"

class DxilEntryPoints : public sub_objects::SubObject
{
public:
	DxilEntryPoints(std::vector<std::string>& entryPoints, MWCptr<IDxcBlob> blob);
	DxilEntryPoints(std::vector<std::wstring>& entryPoints, MWCptr<IDxcBlob> blob);
	~DxilEntryPoints();

	const D3D12_STATE_SUBOBJECT& Get() const;
	void SetEntryPoints(std::vector<std::string>& entryPoints, MWCptr<IDxcBlob> blob);
	void SetEntryPoints(std::vector<std::wstring>& entryPoints, MWCptr<IDxcBlob> blob);
private:
	MWCptr<IDxcBlob> mShaderBlob;
	std::vector<std::wstring> mEntryPointNames;
	std::vector<D3D12_EXPORT_DESC> mExportsDesc;
	D3D12_DXIL_LIBRARY_DESC mDxilLib;
	D3D12_STATE_SUBOBJECT mLibSubObject;
};

