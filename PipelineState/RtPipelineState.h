#pragma once
#include "Comptr.h"
#include <memory>
#include <vector>

namespace sub_objects {
	class SubObject;
}

struct ID3D12Device5;
struct ID3D12StateObject;
struct D3D12_STATE_SUBOBJECT;

class RtPipelineState
{
public:
	RtPipelineState();
	~RtPipelineState();

	void AddSubObject(const std::shared_ptr<sub_objects::SubObject>& subObject);
	void CreatePipelineState(const MWCptr<ID3D12Device5>& device);
	const D3D12_STATE_SUBOBJECT& GetSubObject(unsigned int index) const;
	MWCptr<ID3D12StateObject> GetPipelineState() const;
	unsigned int GetIndex() const;
	void Init();
private:
	MWCptr<ID3D12StateObject> mPipelineState;
	std::vector<D3D12_STATE_SUBOBJECT> mSubObjects;
	unsigned int mIndex;
};

