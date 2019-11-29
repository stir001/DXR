#include <d3d12.h>
#include "AccelerationStructure.h"
#include "D3DCreateHelper.h"

AccelerationStructure::AccelerationStructure()
{
}

AccelerationStructure::~AccelerationStructure()
{
}

AccelerationStructure::ASBuffer AccelerationStructure::InitTopLevelAS(const MWCptr<ID3D12Device5>& device, const MWCptr<ID3D12GraphicsCommandList4>& commandList, const std::vector<D3D12_RAYTRACING_INSTANCE_DESC>& instanceDescs)
{
	D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS input = {};
	input.DescsLayout = D3D12_ELEMENTS_LAYOUT::D3D12_ELEMENTS_LAYOUT_ARRAY;//配列かポインタの配列かを設定できる
	input.Flags = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAGS::D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_NONE;//場合によるパフォーマンス向上のためのフラグ今なしにするが今後は調整対象
	input.NumDescs = instanceDescs.size();
	input.Type = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE::D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL;

	D3D12_RAYTRACING_ACCELERATION_STRUCTURE_PREBUILD_INFO info;
	device->GetRaytracingAccelerationStructurePrebuildInfo(&input, &info);

	ASBuffer asBuffer;

	asBuffer.scratch = d3d_create_helper::CreateBuffer(device, info.ScratchDataSizeInBytes,
		D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_UNORDERED_ACCESS, d3d_create_helper::GetDefaultHeapProps());
	asBuffer.result = d3d_create_helper::CreateBuffer(device, info.ResultDataMaxSizeInBytes,
		D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_RAYTRACING_ACCELERATION_STRUCTURE, d3d_create_helper::GetDefaultHeapProps());//CreateBuffer() info.ResultDataMaxSizeInBytes UA STATE_AS DEFAULT_HEAP

	//CreateBuffer() sizeof(D3D12_RAYTRACING_INSTANCE_DESC) FLAGS_NONE GENERIC_READ UPLOAD_HEAP
	asBuffer.instanceDesc = d3d_create_helper::CreateBuffer(device, sizeof(D3D12_RAYTRACING_INSTANCE_DESC) * instanceDescs.size(),
		D3D12_RESOURCE_FLAG_NONE, D3D12_RESOURCE_STATE_GENERIC_READ, d3d_create_helper::GetUploadHeapProps());
	
	//TODO:インスタンスDescの生成は個別でやるべき？それぞれのモデル情報と関連付けるほうがいいかもしれない
	D3D12_RAYTRACING_INSTANCE_DESC* instanceDesc;
	asBuffer.instanceDesc->Map(0, nullptr, reinterpret_cast<void**>(&instanceDesc));
	memcpy(instanceDesc, instanceDescs.data(), sizeof(instanceDescs[0]) * instanceDescs.size());
	asBuffer.instanceDesc->Unmap(0, nullptr);

	D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_DESC asDesc = {};
	asDesc.Inputs = input;
	asDesc.Inputs.InstanceDescs = asBuffer.instanceDesc->GetGPUVirtualAddress();
	asDesc.DestAccelerationStructureData = asBuffer.result->GetGPUVirtualAddress();
	asDesc.ScratchAccelerationStructureData = asBuffer.scratch->GetGPUVirtualAddress();

	commandList->BuildRaytracingAccelerationStructure(&asDesc, 0, nullptr);

	D3D12_RESOURCE_BARRIER uavBarrier = {};
	uavBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_UAV;
	uavBarrier.UAV.pResource = asBuffer.result.Get();
	commandList->ResourceBarrier(1, &uavBarrier);

	//resultBufferが本体？
	//resource系はExcuteCommandListsを実行するまで保持しておかなくていけないので構造体化して返し、外部に保持責任を持たせる
	return asBuffer;
}

AccelerationStructure::ASBuffer AccelerationStructure::InitBottomLevelTriangleAS(const MWCptr<ID3D12Device5>& device, const MWCptr<ID3D12GraphicsCommandList4>& commandList, const D3D12_RAYTRACING_GEOMETRY_DESC* geometrys, unsigned int geometryCount)
{
	D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS inputs;
	inputs.DescsLayout = D3D12_ELEMENTS_LAYOUT::D3D12_ELEMENTS_LAYOUT_ARRAY;
	inputs.Flags = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_NONE;//BottomLevelではほとんど変更を行わないようにするためFastTraceフラグを立てる
	inputs.NumDescs = geometryCount;
	inputs.pGeometryDescs = geometrys;//マテリアルごとに別々にするのがいい？
	inputs.Type = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL;

	D3D12_RAYTRACING_ACCELERATION_STRUCTURE_PREBUILD_INFO info = {};
	device->GetRaytracingAccelerationStructurePrebuildInfo(&inputs, &info);

	ASBuffer asBuffer;

	asBuffer.scratch = d3d_create_helper::CreateBuffer(device, info.ScratchDataSizeInBytes,
		D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS,
		D3D12_RESOURCE_STATE_UNORDERED_ACCESS, d3d_create_helper::GetDefaultHeapProps());
	//info.ResultDataMaxSizeInBytes FLAG_UA STATE_RAYTRACING_AS HEAP_DEFAULT　256byteアライメントが必須?
	asBuffer.result = d3d_create_helper::CreateBuffer(device, info.ResultDataMaxSizeInBytes,
		D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS,
		D3D12_RESOURCE_STATE_RAYTRACING_ACCELERATION_STRUCTURE,d3d_create_helper::GetDefaultHeapProps());

	D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_DESC asDesc = {};
	asDesc.Inputs = inputs;
	asDesc.DestAccelerationStructureData = asBuffer.result->GetGPUVirtualAddress();
	asDesc.ScratchAccelerationStructureData = asBuffer.scratch->GetGPUVirtualAddress();

	commandList->BuildRaytracingAccelerationStructure(&asDesc, 0, nullptr);

	D3D12_RESOURCE_BARRIER uavBarrier = {};
	uavBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_UAV;
	uavBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAGS::D3D12_RESOURCE_BARRIER_FLAG_NONE;
	uavBarrier.UAV.pResource = asBuffer.result.Get();
	commandList->ResourceBarrier(1, &uavBarrier);

	return asBuffer;
}

D3D12_RAYTRACING_GEOMETRY_DESC AccelerationStructure::CreateTriangleGeometryDesc(const TriangleBuffers& buffers, const D3D12_RAYTRACING_GEOMETRY_FLAGS& flags)
{
	D3D12_RAYTRACING_GEOMETRY_DESC geometryDesc = {};
	if (buffers.vertexBuffer == nullptr)
	{
		return geometryDesc;
	}
	D3D12_GPU_VIRTUAL_ADDRESS indexBufferVirtualAddress = buffers.indexBuffer != nullptr ? buffers.indexBuffer->GetGPUVirtualAddress() : 0;
	geometryDesc.Type = D3D12_RAYTRACING_GEOMETRY_TYPE_TRIANGLES;
	geometryDesc.Flags = flags;
	geometryDesc.Triangles.VertexBuffer.StartAddress = buffers.vertexBuffer->GetGPUVirtualAddress();
	geometryDesc.Triangles.VertexBuffer.StrideInBytes = buffers.vertexStride;
	geometryDesc.Triangles.VertexCount = buffers.vertexCount;
	geometryDesc.Triangles.VertexFormat = buffers.vertexFormat;
	geometryDesc.Triangles.IndexBuffer = indexBufferVirtualAddress;
	geometryDesc.Triangles.IndexCount = buffers.indexCount;
	geometryDesc.Triangles.IndexFormat = buffers.indexFormat;
	geometryDesc.Triangles.Transform3x4 = 0;//AS作成時に適応するための3x4のアフィン行列を保持したBufferのvirtualAddress nullptr可能
	return geometryDesc;
}
