#pragma once
#include "SubObject.h"
#include "Comptr.h"
#include <d3d12.h>


class RtRootSignature :
	public sub_objects::SubObject
{
public:
	RtRootSignature(const MWCptr<ID3D12Device5>& device, const D3D12_ROOT_SIGNATURE_DESC& desc, bool isLocal = true);
	RtRootSignature(const MWCptr<ID3D12RootSignature>& rootSignature, bool isLocal = true);
	~RtRootSignature();

	void Init(const MWCptr<ID3D12Device5>& device, const D3D12_ROOT_SIGNATURE_DESC& desc, bool isLocal = true);
	const D3D12_STATE_SUBOBJECT& Get() const;
private:
	D3D12_STATE_SUBOBJECT mSubObject;
	MWCptr<ID3D12RootSignature> mRootSignature;
	ID3D12RootSignature* mInterface;
};

