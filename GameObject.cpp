#include "GameObject.h"
#include "D3DCreateHelper.h"
#include "AccelerationStructure/AccelerationStructure.h"
#include "SubObjects/SubObjectsCreate.h"
#include <d3d12.h>

GameObject::GameObject(std::vector<D3D12_RAYTRACING_INSTANCE_DESC>& instanceDescs, const MWCptr<ID3D12Device5>& device, const MWCptr<ID3D12GraphicsCommandList4>& cmdList,
	D3DDescriptorHeap& heap, unsigned int hitGroupIndex)
	: mInstanceDescs(instanceDescs), mDevice(device), mCmdList(cmdList), mHitGroupIndex(hitGroupIndex), mCbvSrvUavHeap(heap)
{
	InitTriangle();
}

const AccelerationStructure::ASBuffer& GameObject::GetBLAS() const
{
	return mBLAS;
}

void GameObject::Draw()
{
}

void GameObject::InitTriangle()
{
	struct Vector3
	{
		float x;
		float y;
		float z;
	};

	struct Triangle
	{
		Vector3 pos1;
		Vector3 pos2;
		Vector3 pos3;
	};

	mVertexBuffer = d3d_create_helper::CreateBuffer(mDevice,
		sizeof(Triangle), D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE,
		D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ,
		d3d_create_helper::GetUploadHeapProps());
	mVertexBuffer->SetName(L"vertexBuffer");

	Triangle tri = { {0.0f, 1.0f, 0.0f}
					,{0.866f,  -0.5f, 0}
					,{-0.866f, -0.5f, 0}
	};

	Triangle* data;
	mVertexBuffer->Map(0, nullptr,(void**)&data);
	*data = tri;
	mVertexBuffer->Unmap(0, nullptr);

	AccelerationStructure::TriangleBuffers buffers = {};
	buffers.indexBuffer = nullptr;
	buffers.vertexBuffer = mVertexBuffer;
	buffers.vertexCount = 3;
	buffers.vertexFormat = DXGI_FORMAT_R32G32B32_FLOAT;
	buffers.vertexStride = sizeof(Vector3);

	mGeoDescs.resize(1);
	mGeoDescs[0] = AccelerationStructure::CreateTriangleGeometryDesc(buffers, D3D12_RAYTRACING_GEOMETRY_FLAG_OPAQUE);
	mBLAS = AccelerationStructure::InitBottomLevelTriangleAS(mDevice, mCmdList, mGeoDescs.data(), mGeoDescs.size());
	mBLAS.result->SetName(L"BLAS");
	CreateInstanceDesc();
}

void GameObject::CreateInstanceDesc()
{
	float mat[3][4] = { 
		{1.0f, 0.0f, 0.0f, 0.0f},
		{0.0f, 1.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 1.0f, 0.0f} 
	};

	D3D12_RAYTRACING_INSTANCE_DESC instanceDesc;

	instanceDesc.AccelerationStructure = mBLAS.result->GetGPUVirtualAddress();
	instanceDesc.Flags = D3D12_RAYTRACING_INSTANCE_FLAGS::D3D12_RAYTRACING_INSTANCE_FLAG_NONE;
	instanceDesc.InstanceContributionToHitGroupIndex = mHitGroupIndex;
	instanceDesc.InstanceID = 0;
	instanceDesc.InstanceMask = 0xff;
	memcpy(instanceDesc.Transform, &mat, sizeof(mat));

	mInstanceDescs.push_back(instanceDesc);

	//for (unsigned int i = 0; i < mInstanceDescs.size(); ++i)
	//{
	//	mInstanceDesc[i].AccelerationStructure = mBLAS.result->GetGPUVirtualAddress();
	//	mInstanceDesc[i].Flags = D3D12_RAYTRACING_INSTANCE_FLAGS::D3D12_RAYTRACING_INSTANCE_FLAG_NONE;
	//	mInstanceDesc[i].InstanceContributionToHitGroupIndex = mHitGroupIndex;
	//	mInstanceDesc[i].InstanceID = i;
	//	mInstanceDesc[i].InstanceMask = 0xff;
	//	memcpy(mInstanceDesc[i].Transform, &mat, sizeof(mat));
	//}
}
