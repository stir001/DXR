#include "DrawObjectCreator.h"
#include "AccelerationStructure/AccelerationStructure.h"
#include "GameObject.h"
#include <d3d12.h>

DrawObjectCreator::DrawObjectCreator(D3DDescriptorHeap& heap, const MWCptr<ID3D12Device5>& device, const MWCptr<ID3D12GraphicsCommandList4>& commandList)
	: mCbvSrvUavHeap(heap), mCommandList(commandList), mDevice(device)
{
}

std::shared_ptr<GameObject> DrawObjectCreator::CreateGameObject(unsigned int hitGroupIndex)
{
	return std::make_shared<GameObject>(mInstanceDescs, mDevice, mCommandList, mCbvSrvUavHeap, hitGroupIndex);
}

AccelerationStructure::ASBuffer DrawObjectCreator::CreateTLAS()
{
	mTLAS = AccelerationStructure::InitTopLevelAS(mDevice, mCommandList, mInstanceDescs);
	mTLAS.result->SetName(L"TLAS");
	return mTLAS;
}

AccelerationStructure::ASBuffer DrawObjectCreator::GetTLAS() const
{
	return mTLAS;
}
