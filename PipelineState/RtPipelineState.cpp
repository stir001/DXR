#include "RtPipelineState.h"
#include "SubObjects/SubObject.h"
#include "D3DCreateHelper.h"
#include <d3d12.h>

RtPipelineState::RtPipelineState() : mPipelineState(nullptr), mIndex(0U)
{
	Init();
}

RtPipelineState::~RtPipelineState()
{
}

void RtPipelineState::AddSubObject(const std::shared_ptr<sub_objects::SubObject>& subObject)
{
	mSubObjects.push_back(subObject->Get());
	++mIndex;
}

void RtPipelineState::CreatePipelineState(const MWCptr<ID3D12Device5>& device)
{
	D3D12_STATE_OBJECT_DESC desc;
	desc.NumSubobjects = mIndex;
	desc.pSubobjects = mSubObjects.data();
	desc.Type = D3D12_STATE_OBJECT_TYPE_RAYTRACING_PIPELINE;

	d3d_create_helper::D3DError(device->CreateStateObject(&desc, IID_PPV_ARGS(&mPipelineState)));
}

const D3D12_STATE_SUBOBJECT& RtPipelineState::GetSubObject(unsigned int index) const
{
	return mSubObjects[index];
}

MWCptr<ID3D12StateObject> RtPipelineState::GetPipelineState() const
{
	return mPipelineState;
}

unsigned int RtPipelineState::GetIndex() const
{
	return mIndex;
}

void RtPipelineState::Init()
{
	mSubObjects.clear();
	mPipelineState = nullptr;
	mIndex = 0;
}
