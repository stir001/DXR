#include "DrawObjectCreator.h"
#include "AccelerationStructure/AccelerationStructure.h"
#include "GameObject.h"
#include "FMDLoader.h"
#include "FMDdata.h"
#include "FmdGameObject.h"
#include "ChessBoard.h"
#include "D3DCreateHelper.h"
#include "D3DDescriptorHeap.h"
#include <d3d12.h>

DrawObjectCreator::DrawObjectCreator(D3DDescriptorHeap& heap, const MWCptr<ID3D12Device5>& device, const MWCptr<ID3D12GraphicsCommandList4>& commandList
	, const std::shared_ptr<ShaderTable>& shaderTable)
	: mCbvSrvUavHeap(heap), mCommandList(commandList), mDevice(device), mShaderTable(shaderTable)
{
}

std::shared_ptr<GameObject> DrawObjectCreator::LoadChess(const std::string& path, const Vector4& color)
{
	Fmd::FMDLoader loader;
	auto data = loader.LoadFMD(path);
	auto obj = std::make_shared<FmdGameObject>(data, mInstanceDescs, mDevice, mCommandList, mCbvSrvUavHeap, mShaderTable, color);
	return obj;
}

std::shared_ptr<GameObject> DrawObjectCreator::LoadChess(const std::string& path, const Vector4& color, const Matrix& initMat)
{
	Fmd::FMDLoader loader;
	auto data = loader.LoadFMD(path);
	auto obj = std::make_shared<FmdGameObject>(data, mInstanceDescs, mDevice, mCommandList, mCbvSrvUavHeap, mShaderTable, color, initMat);
	return obj;
}

std::shared_ptr<GameObject> DrawObjectCreator::CreateChessBoard()
{
	std::shared_ptr<GameObject> rtn = std::make_shared<ChessBoard>(mInstanceDescs, mDevice, mCommandList, mCbvSrvUavHeap, mShaderTable);
	return rtn;
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

unsigned int DrawObjectCreator::GetTLASHeapIndex() const
{
	return mTLASHeapIndex;
}

void DrawObjectCreator::SetTLAS()
{
	auto desc = d3d_create_helper::CreateSRVAS(mTLAS.result);
	mTLASHeapIndex = mCbvSrvUavHeap.AddViewDesc(desc, nullptr);
}
