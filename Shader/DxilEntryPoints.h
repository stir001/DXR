#pragma once
#include <vector>
#include <string>
#include <d3d12.h>
#include <dxcapi.h>
#include "Comptr.h"

class DxilEntryPoints
{
public:
	DxilEntryPoints(std::vector<std::string>& entryPoints, MWCptr<IDxcBlob> blob);
	DxilEntryPoints(std::vector<std::wstring>& entryPoints, MWCptr<IDxcBlob> blob);
	~DxilEntryPoints();

	void SetEntryPoints(std::vector<std::string>& entryPoints, MWCptr<IDxcBlob> blob);
	void SetEntryPoints(std::vector<std::wstring>& entryPoints, MWCptr<IDxcBlob> blob);
	const D3D12_STATE_SUBOBJECT& GetDxilLibrary() const;
private:
	MWCptr<IDxcBlob> mShaderBlob;
	std::vector<std::wstring> mEntryPointNames;
	std::vector<D3D12_EXPORT_DESC> mExportsDesc;
	D3D12_DXIL_LIBRARY_DESC mDxilLib;
	D3D12_STATE_SUBOBJECT mLibSubObject;
};

