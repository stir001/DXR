#pragma once
#include "SubObjects/SubObject.h"
#include "Comptr.h"
#include <d3d12.h>
#include <dxcapi.h>
#include <vector>
#include <string>

class DxilLibrarySubobjects :
	public sub_objects::SubObject
{
public:
	DxilLibrarySubobjects(const MWCptr<IDxcBlob>& blob);
	const D3D12_STATE_SUBOBJECT& Get() const;
	
private:
	void Init();
	MWCptr<IDxcBlob> mShaderBlob;
	std::vector<std::wstring> mExportNames;
	std::vector<D3D12_EXPORT_DESC> mExportsDesc;
	D3D12_DXIL_LIBRARY_DESC mDxilLib;
	D3D12_STATE_SUBOBJECT mSubObject;
};

