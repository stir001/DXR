#include "FmdGameObject.h"
#include "FMDdata.h"
#include "D3DCreateHelper.h"
#include "Shader/ShaderTable.h"
#include "D3DDescriptorHeap.h"
#include "Mathatic.h"
#include <DirectXMath.h>
#include <d3d12.h>

FmdGameObject::FmdGameObject(const Fmd::FMDFileData& data, std::vector<D3D12_RAYTRACING_INSTANCE_DESC>& instanceDescs, const MWCptr<ID3D12Device5>& device,
	const MWCptr<ID3D12GraphicsCommandList4>& cmdList, D3DDescriptorHeap& heap, std::shared_ptr<ShaderTable>& shaderTable, const Vector4& color)
	: GameObject(instanceDescs), mColor(color)
{
	InitFmd(data, device, cmdList, heap, shaderTable, Matrix::Identity());
}

FmdGameObject::FmdGameObject(const Fmd::FMDFileData& data, std::vector<D3D12_RAYTRACING_INSTANCE_DESC>& instanceDescs, const MWCptr<ID3D12Device5>& device, const MWCptr<ID3D12GraphicsCommandList4>& cmdList, D3DDescriptorHeap& heap, std::shared_ptr<ShaderTable>& shaderTable, const Vector4& color, const Matrix& initMatrix)
	: GameObject(instanceDescs), mColor(color)
{
	InitFmd(data, device, cmdList, heap, shaderTable, initMatrix);
}

void FmdGameObject::InitFmd(const Fmd::FMDFileData& data, const MWCptr<ID3D12Device5>& device,
	const MWCptr<ID3D12GraphicsCommandList4>& cmdList, D3DDescriptorHeap& heap, std::shared_ptr<ShaderTable>& shaderTable, const Matrix& initMatrix)
{
	unsigned int vertexDataSize = data.vertices.size() * sizeof(data.vertices[0]);
	std::vector<Vector3> vertex(data.vertexNum);

	for (unsigned int i = 0; i < data.vertexNum; ++i)
	{
		memcpy(&vertex[i], &data.vertices[i].pos, sizeof(Vector3));
	}

	InitVertices(vertex, initMatrix);

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
	InitNormals(normals, initMatrix);

	mNormalBuffer = d3d_create_helper::CreateBuffer(device, sizeof(normals[0]) * normals.size()
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

	mMaterialBuffer = d3d_create_helper::CreateBuffer(device, d3d_create_helper::Alignment(256, sizeof(mColor)), D3D12_RESOURCE_FLAG_NONE, D3D12_RESOURCE_STATE_GENERIC_READ, d3d_create_helper::GetUploadHeapProps());
	{
		void* data = nullptr;
		mMaterialBuffer->Map(0, nullptr, &data);
		memcpy(data, &mColor, sizeof(mColor));
		mMaterialBuffer->Unmap(0, nullptr);

		D3D12_CONSTANT_BUFFER_VIEW_DESC cbv = {};
		cbv.BufferLocation = mMaterialBuffer->GetGPUVirtualAddress();
		cbv.SizeInBytes = d3d_create_helper::Alignment(256, sizeof(mColor));
		auto heapindex = heap.AddViewDesc(cbv);
	}

	ShaderTable::ShaderInfo transInfo = {};
	transInfo.heapIndices.push_back(normalHeapIndex);
	transInfo.name = L"TransHitGroup";
	auto hitGroupOffset = shaderTable->AddHitGroupShader(transInfo);

	ShaderTable::ShaderInfo reflectInfo;
	reflectInfo.heapIndices.push_back(normalHeapIndex);
	reflectInfo.name = L"ReflectHitGroup";
	shaderTable->AddHitGroupShader(reflectInfo);

	ShaderTable::ShaderInfo info = {};
	info.heapIndices.push_back(normalHeapIndex);
	info.name = L"ChessHitGroup";

	shaderTable->AddHitGroupShader(info);
	CreateInstanceDesc(mInstanceDescs, hitGroupOffset);
}

void FmdGameObject::InitVertices(std::vector<Vector3>& vertices, const Matrix& mat)
{
	if (mat == Matrix::Identity())
	{
		return;
	}

	Vector3 maxVal = {-FLT_MAX,-FLT_MAX ,-FLT_MAX };
	Vector3 minVal = { FLT_MAX,FLT_MAX ,FLT_MAX };

	for (auto& v : vertices)
	{
		v = mat * v;
		maxVal.x = maxVal.x < v.x ? v.x : maxVal.x;
		maxVal.y = maxVal.y < v.y ? v.y : maxVal.y;
		maxVal.z = maxVal.z < v.z ? v.z : maxVal.z;

		minVal.x = minVal.x > v.x ? v.x : minVal.x;
		minVal.y = minVal.y > v.y ? v.y : minVal.y;
		minVal.z = minVal.z > v.z ? v.z : minVal.z;
	}

	Vector3 bottomPos = { (maxVal.x + minVal.x) * 0.5f, minVal.y, (maxVal.z + minVal.z) * 0.5f };

	for (auto& v : vertices)
	{
		v -= bottomPos;
	}
}

void FmdGameObject::InitNormals(std::vector<Vector3>& normals, const Matrix& mat)
{
	if (mat == Matrix::Identity())
	{
		return;
	}
	for (auto& n : normals)
	{
		n = mat * n;
	}
}

unsigned int FmdGameObject::AddTransHitGroups(std::shared_ptr<ShaderTable>& shaderTable, unsigned int heapIndex, unsigned int addCount)
{
	ShaderTable::ShaderInfo info = {};
	info.heapIndices.push_back(heapIndex);
	info.name = L"TransHitGroup";
	unsigned int hitGroupIndex = shaderTable->AddHitGroupShader(info);

	for (unsigned int i = 0; i < addCount - 1; ++i)
	{
		shaderTable->AddHitGroupShader(info);
	}

	return hitGroupIndex;
}

