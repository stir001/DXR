#pragma once
#include "SubObject.h"
#include <d3d12.h>
#include <string>
#include <vector>

class ExportAssociation :
	public sub_objects::SubObject
{
public:
	ExportAssociation(std::vector<std::wstring>& exportNames, D3D12_STATE_SUBOBJECT& subobjectToAssociation);
	~ExportAssociation();

	D3D12_STATE_SUBOBJECT Get() const;

	//exportNamesÇÕstateobjectÇçÏê¨Ç∑ÇÈÇ‹Ç≈ï€éùÇ∑ÇÈÇ±Ç∆
	void Init(std::vector<std::wstring>& exportNames, D3D12_STATE_SUBOBJECT& subobjectToAssociation);
private:
	D3D12_STATE_SUBOBJECT mSubObject;
	D3D12_SUBOBJECT_TO_EXPORTS_ASSOCIATION mAssociation;
	std::vector<LPCWSTR> mNames;
};

