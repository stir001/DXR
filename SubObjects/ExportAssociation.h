#pragma once
#include "SubObject.h"
#include <d3d12.h>
#include <string>
#include <vector>

class ExportAssociation :
	public sub_objects::SubObject
{
public:
	ExportAssociation(const std::vector<std::wstring>& exportNames, unsigned int subOjectIndex);
	~ExportAssociation();

	const D3D12_STATE_SUBOBJECT& Get() const;

	//exportNames��stateobject���쐬����܂ŕێ����邱��
	void Init(const std::vector<std::wstring>& exportNames, unsigned int subOjectIndex);
	void Create(RtPipelineState& pipelineState);
private:
	void SetSubObject(RtPipelineState& pipelineState);
	D3D12_STATE_SUBOBJECT mSubObject;
	D3D12_SUBOBJECT_TO_EXPORTS_ASSOCIATION mAssociation;
	std::vector<LPCWSTR> mNames;
	LPCWSTR* mNamePtr;
	unsigned int mAssociateIndex;
};

