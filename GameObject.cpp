#include "GameObject.h"
#include "D3DCreateHelper.h"
#include "AccelerationStructure/AccelerationStructure.h"
#include "SubObjects/SubObjectsCreate.h"
#include "FMDdata.h"
#include "Mathatic.h"
#include "D3DDescriptorHeap.h"
#include "Shader/ShaderTable.h"
#include <d3d12.h>


GameObject::GameObject(const Fmd::FMDFileData& data, std::vector<D3D12_RAYTRACING_INSTANCE_DESC>& instanceDescs, const MWCptr<ID3D12Device5>& device,
	const MWCptr<ID3D12GraphicsCommandList4>& cmdList, D3DDescriptorHeap& heap, std::shared_ptr<ShaderTable>& shaderTable) 
	: mMat(Matrix::Identity()), mInstanceDescs(instanceDescs)
{
	InitFmd(data, device, cmdList, heap, shaderTable);
}

const AccelerationStructure::ASBuffer& GameObject::GetBLAS() const
{
	return mBLAS;
}

void GameObject::SetPos(const Vector3& pos)
{
	mMat._03 = pos.x;
	mMat._13 = pos.y;
	mMat._23 = pos.z;
	UpdateMatrix();

}

void GameObject::AddPos(const Vector3& val)
{
	Matrix::Translate(val);
}

void GameObject::Draw()
{
}

void GameObject::InitFmd(const Fmd::FMDFileData& data, const MWCptr<ID3D12Device5>& device,
	const MWCptr<ID3D12GraphicsCommandList4>& cmdList, D3DDescriptorHeap& heap, std::shared_ptr<ShaderTable>& shaderTable)
{
	unsigned int vertexDataSize = data.vertices.size() * sizeof(data.vertices[0]);
	std::vector<Vector3> vertex(data.vertexNum);

	for (unsigned int i = 0; i < data.vertexNum; ++i)
	{
		memcpy(&vertex[i], &data.vertices[i].pos, sizeof(Vector3));
	}

	mVertexBuffer = d3d_create_helper::CreateBuffer(device,
		data.vertexNum * sizeof(vertex[0]),
		D3D12_RESOURCE_FLAG_NONE, D3D12_RESOURCE_STATE_GENERIC_READ,
		d3d_create_helper::GetUploadHeapProps());
	mVertexBuffer->SetName(L"ModelVertex");
	{
		void* map = nullptr;
		mVertexBuffer->Map(0, nullptr, &map);
		memcpy(map, vertex.data(), sizeof(vertex[0]) * data.vertexNum);
		mVertexBuffer->Unmap(0, nullptr);
	}

	mIndexBuffer = d3d_create_helper::CreateBuffer(device,
		data.indexNum * sizeof(data.indices[0]),
		D3D12_RESOURCE_FLAG_NONE, D3D12_RESOURCE_STATE_GENERIC_READ,
		d3d_create_helper::GetUploadHeapProps());
	mIndexBuffer->SetName(L"ModelIndices");
	{
		void* map = nullptr;
		mIndexBuffer->Map(0, nullptr, &map);
		memcpy(map, data.indices.data(), sizeof(data.indices[0]) * data.indexNum);
		mIndexBuffer->Unmap(0, nullptr);
	}

	AccelerationStructure::TriangleBuffers buffers = {};
	buffers.indexBuffer = mIndexBuffer;
	buffers.indexCount = data.indexNum;
	buffers.indexFormat = DXGI_FORMAT_R32_UINT;
	buffers.vertexBuffer = mVertexBuffer;
	buffers.vertexCount = data.vertexNum;
	buffers.vertexFormat = DXGI_FORMAT_R32G32B32_FLOAT;
	buffers.vertexStride = sizeof(vertex[0]);
	mGeoDescs.resize(1);
	mGeoDescs[0] = AccelerationStructure::CreateTriangleGeometryDesc(buffers, D3D12_RAYTRACING_GEOMETRY_FLAG_OPAQUE);
	mBLAS = AccelerationStructure::InitBottomLevelTriangleAS(device, cmdList, mGeoDescs.data(), mGeoDescs.size());

	std::vector<Vector3> normals(data.indexNum / 3);
	for (unsigned int i = 0; i < data.indexNum;) {
		Vector3 normal1;
		memcpy(&normal1, &data.vertices[data.indices[i++]].normal, sizeof(Vector3));
		Vector3 normal2;
		memcpy(&normal2, &data.vertices[data.indices[i++]].normal, sizeof(Vector3));
		Vector3 normal3;
		memcpy(&normal3, &data.vertices[data.indices[i++]].normal, sizeof(Vector3));
		normals[(i - 3) / 3] = (normal1 + normal2 + normal3) / 3.0f;
	}

	mNormalBuffer = d3d_create_helper::CreateBuffer(device, sizeof(Vector3) * normals.size()
		, D3D12_RESOURCE_FLAG_NONE, D3D12_RESOURCE_STATE_GENERIC_READ, d3d_create_helper::GetUploadHeapProps());
	mNormalBuffer->SetName(L"NormalBuffer");
	{
		void* map = nullptr;
		mNormalBuffer->Map(0, nullptr, &map);
		memcpy(map, normals.data(), sizeof(normals[0]) * normals.size());
		mNormalBuffer->Unmap(0, nullptr);
	}
	D3D12_SHADER_RESOURCE_VIEW_DESC desc = {};
	desc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	desc.Buffer.NumElements = normals.size();
	desc.Format = DXGI_FORMAT_UNKNOWN;
	desc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	desc.Buffer.StructureByteStride = sizeof(normals[0]);
	auto normalHeapIndex = heap.AddViewDesc(desc, mNormalBuffer);

	ShaderTable::ShaderInfo info = {};
	info.heapIndices.push_back(normalHeapIndex);
	info.name = L"NormalHitGroup";

	auto hitGroupOffset = shaderTable->AddHitGroupShader(info);
	CreateInstanceDesc(mInstanceDescs, hitGroupOffset);
}

void GameObject::CreateInstanceDesc(std::vector<D3D12_RAYTRACING_INSTANCE_DESC>& instanceDescs, unsigned int hitGroupIndex)
{
	float mat[3][4];
	memcpy(mat, &mMat, sizeof(float) * 3 * 4);

	D3D12_RAYTRACING_INSTANCE_DESC instanceDesc;

	instanceDesc.AccelerationStructure = mBLAS.result->GetGPUVirtualAddress();
	instanceDesc.Flags = D3D12_RAYTRACING_INSTANCE_FLAGS::D3D12_RAYTRACING_INSTANCE_FLAG_NONE;
	instanceDesc.InstanceContributionToHitGroupIndex = hitGroupIndex;
	instanceDesc.InstanceID = instanceDescs.size();
	instanceDesc.InstanceMask = 0xff;
	memcpy(instanceDesc.Transform, mat, sizeof(mat));

	mInstansDescIndex = instanceDescs.size();
	instanceDescs.push_back(instanceDesc);
}

void GameObject::UpdateMatrix()
{
	float mat[3][4];
	memcpy(mat, &mMat, sizeof(float) * 3 * 4);
	memcpy(mInstanceDescs[mInstansDescIndex].Transform, mat, sizeof(mat));
}
