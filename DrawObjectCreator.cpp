#include "DrawObjectCreator.h"
#include "AccelerationStructure/AccelerationStructure.h"
#include "GameObject.h"
#include "FMDLoader.h"
#include "FMDdata.h"
#include <d3d12.h>

DrawObjectCreator::DrawObjectCreator(D3DDescriptorHeap& heap, const MWCptr<ID3D12Device5>& device, const MWCptr<ID3D12GraphicsCommandList4>& commandList
	, const std::shared_ptr<ShaderTable>& shaderTable)
	: mCbvSrvUavHeap(heap), mCommandList(commandList), mDevice(device), mShaderTable(shaderTable)
{
}

std::shared_ptr<GameObject> DrawObjectCreator::LoadFMD(const std::string& path)
{
	Fmd::FMDLoader loader;
	auto data = loader.LoadFMD(path);
	auto obj = std::make_shared<GameObject>(data, mInstanceDescs, mDevice, mCommandList, mCbvSrvUavHeap, mShaderTable);
	return obj;
}

void DrawObjectCreator::AddHitGroupPattern(const DrawObject::HitGroupPattern& pattern)
{
	mHitGroupPattern.push_back(pattern);
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

void DrawObjectCreator::UpdateTLAS()
{
	mTLAS = AccelerationStructure::UpdateTopLevelAS(mDevice, mCommandList, mInstanceDescs, mTLAS);
}
