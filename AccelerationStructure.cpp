#include <d3d12.h>
#include "AccelerationStructure.h"

AccelearationStructure::AccelearationStructure()
{
}

AccelearationStructure::~AccelearationStructure()
{
}

AccelearationStructure::ASBuffer AccelearationStructure::InitTopLevelAS(MWCptr<ID3D12Device5>& device, MWCptr< ID3D12GraphicsCommandList4>& commandList, MWCptr<ID3D12Resource>& bottomLevelAS)
{
	D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS input = {};
	input.DescsLayout = D3D12_ELEMENTS_LAYOUT::D3D12_ELEMENTS_LAYOUT_ARRAY;//配列かポインタの配列かを設定できる
	input.Flags = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAGS::D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_NONE;//場合によるパフォーマンス向上のためのフラグ
	input.NumDescs = 1;
	input.Type = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE::D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL;

	D3D12_RAYTRACING_ACCELERATION_STRUCTURE_PREBUILD_INFO info;
	device->GetRaytracingAccelerationStructurePrebuildInfo(&input, &info);

	ASBuffer asBuffer;

	asBuffer.scratch = nullptr;//CreateBuffer() info.ScratchDataSizeInBytes UA STATE_UA DEFAULT_HEAP
	asBuffer.result = nullptr;//CreateBuffer() info.ResultDataMaxSizeInBytes UA STATE_AS DEFAULT_HEAP

	asBuffer.instanceDesc = nullptr;//CreateBuffer() sizeof(D3D12_RAYTRACING_INSTANCE_DESC) FLAGS_NONE GENERIC_READ UPLOAD_HEAP
	
	D3D12_RAYTRACING_INSTANCE_DESC* instanceDesc;
	asBuffer.instanceDesc->Map(0, nullptr, reinterpret_cast<void**>(&instanceDesc));
	instanceDesc->InstanceID = 0;
	instanceDesc->InstanceContributionToHitGroupIndex = 0;
	instanceDesc->Transform;//(identityをとりあえず入れる。ただしfloat[3][4]の配列であることに注意
	instanceDesc->AccelerationStructure = bottomLevelAS->GetGPUVirtualAddress();//256バイトアライメントされたものでないといけない
	instanceDesc->InstanceMask = 0xff;

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

AccelearationStructure::ASBuffer AccelearationStructure::InitBottomLevelTriangleAS(MWCptr<ID3D12Device5>& device, MWCptr<ID3D12GraphicsCommandList4>& commandList, const D3D12_RAYTRACING_GEOMETRY_DESC* geometrys, unsigned int geometryCount)
{
	D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS inputs;
	inputs.DescsLayout = D3D12_ELEMENTS_LAYOUT::D3D12_ELEMENTS_LAYOUT_ARRAY;
	inputs.Flags = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAGS::D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_NONE;
	inputs.NumDescs = geometryCount;
	inputs.pGeometryDescs = geometrys;
	inputs.Type = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL;

	D3D12_RAYTRACING_ACCELERATION_STRUCTURE_PREBUILD_INFO info = {};
	device->GetRaytracingAccelerationStructurePrebuildInfo(&inputs, &info);

	ASBuffer asBuffer;

	asBuffer.scratch = nullptr;//info.ScrathcDataSizeInBytes FLAG_UA STATE_UA HEAP_DEFAULT
	asBuffer.result = nullptr;//info.ResultDataMaxSizeInBytes FLAG_UA STATE_RAYTRACING_AS HEAP_DEFAULT　256byteアライメントが必須?

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

D3D12_RAYTRACING_GEOMETRY_DESC AccelearationStructure::CreateTriangleGeometryDesc(const TriangleBuffers& buffers, D3D12_RAYTRACING_GEOMETRY_FLAGS& flags)
{
	D3D12_RAYTRACING_GEOMETRY_DESC geometryDesc = {};
	geometryDesc.Type = D3D12_RAYTRACING_GEOMETRY_TYPE_TRIANGLES;
	geometryDesc.Flags = flags;
	geometryDesc.Triangles.VertexBuffer.StartAddress = buffers.vertexBuffer->GetGPUVirtualAddress();
	geometryDesc.Triangles.VertexBuffer.StrideInBytes = buffers.vertexStride;
	geometryDesc.Triangles.VertexCount = buffers.vertexCount;
	geometryDesc.Triangles.VertexFormat = buffers.vertexFormat;
	geometryDesc.Triangles.IndexBuffer = buffers.indexBuffer->GetGPUVirtualAddress();
	geometryDesc.Triangles.IndexCount = buffers.indexCount;
	geometryDesc.Triangles.IndexFormat = buffers.indexFormat;
	geometryDesc.Triangles.Transform3x4;
	return geometryDesc;
}
