#include "ChessBoard.h"
#include "D3DCreateHelper.h"
#include "D3DDescriptorHeap.h"
#include "Shader/ShaderTable.h"
#include <d3d12.h>
#include <functional>

struct ChessBoard::Square {
	Vector3 pos1;
	Vector3 pos2;
	Vector3 pos3;
	Vector3 pos4;

	void Add(const Vector3& offset)
	{
		pos1 += offset;
		pos2 += offset;
		pos3 += offset;
		pos4 += offset;
	}

	void Sub(const Vector3& offset)
	{
		Add(-offset);
	}
};

ChessBoard::ChessBoard(std::vector<D3D12_RAYTRACING_INSTANCE_DESC>& instanceDesc, const MWCptr<ID3D12Device5>& device,
	const MWCptr<ID3D12GraphicsCommandList4>& cmdList, D3DDescriptorHeap& heap, std::shared_ptr<ShaderTable>& shaderTable)
	: GameObject(instanceDesc)
{
	CreateChessBoard(device, cmdList, heap, shaderTable);
}

void ChessBoard::CreateChessBoard(const MWCptr<ID3D12Device5>& device, const MWCptr<ID3D12GraphicsCommandList4>& cmdList,
	D3DDescriptorHeap& heap, std::shared_ptr<ShaderTable>& shaderTable)
{
	//8x8のボード
	const unsigned int squarePerLine = 4U;//ラインごとの四角形の数
	const unsigned int vertexPerSquare = 4U;//四角形ごとの頂点数
	const unsigned int lineNum = 8U;//ラインの総数
	std::vector<Vector3> whiteVertices(squarePerLine * lineNum * vertexPerSquare);
	std::vector<Vector3> blackVertices(squarePerLine * lineNum * vertexPerSquare);

	CreateBoard(whiteVertices, blackVertices);
	WriteVertexBuffer(mWhiteVertexBuffer, whiteVertices, device);
	WriteVertexBuffer(mBlackVertexBuffer, blackVertices, device);

	unsigned int indicesPerSquare = 6U;//四角形ごとのインデックス数
	std::vector<unsigned int> whiteIndices(squarePerLine * lineNum * indicesPerSquare);
	std::vector<unsigned int> blackIndices(squarePerLine * lineNum * indicesPerSquare);

	CreateIndices(whiteIndices);
	CreateIndices(blackIndices);

	WriteIndexBuffer(mWhiteIndexBuffer, whiteIndices, device);
	WriteIndexBuffer(mBlackIndexBuffer, blackIndices, device);

	mGeoDescs.resize(2);

	mGeoDescs[0] = CreateTriangleGeometry(mWhiteVertexBuffer, whiteVertices.size(), mWhiteIndexBuffer, whiteIndices.size());
	mGeoDescs[1] = CreateTriangleGeometry(mBlackVertexBuffer, blackVertices.size(), mBlackIndexBuffer, blackIndices.size());

	unsigned int bufferSize = d3d_create_helper::Alignment(256, sizeof(float) * 3);
	unsigned int hitGroupIndex = UINT_MAX;

	mWhiteMaterial = d3d_create_helper::CreateBuffer(device, bufferSize, D3D12_RESOURCE_FLAG_NONE, D3D12_RESOURCE_STATE_GENERIC_READ, d3d_create_helper::GetUploadHeapProps());
	auto whiteHeapIndex = AddHeapColorCBV(mWhiteMaterial, Vector3(1.0f, 1.0f, 1.0f), heap, bufferSize);

	std::function addShaderInfo = [](ShaderTable::ShaderInfo& info, ShaderTable& table, unsigned int addNum) {
		unsigned int rtn = table.GetHitGroupNum();
		if (addNum > 0)
		{
			++rtn;
			for (unsigned int i = 0; i < addNum; ++i)
			{
				table.AddHitGroupShader(info);
			}
		}
		return rtn;
	};

	mBlackMaterial = d3d_create_helper::CreateBuffer(device, bufferSize, D3D12_RESOURCE_FLAG_NONE, D3D12_RESOURCE_STATE_GENERIC_READ, d3d_create_helper::GetUploadHeapProps());
	auto blackHeapIndex = AddHeapColorCBV(mBlackMaterial, Vector3(0.0f, 0.0f, 0.0f), heap, bufferSize);

	ShaderTable::ShaderInfo whiteinfo = {};
	whiteinfo.heapIndices.push_back(whiteHeapIndex);
	whiteinfo.name = L"BoardTransHitGroup";
	hitGroupIndex = addShaderInfo(whiteinfo, *shaderTable, 1);
	whiteinfo.name = L"BoardRefHitGroup";
	addShaderInfo(whiteinfo, *shaderTable, 1);
	whiteinfo.name = L"BoardHitGroup";
	addShaderInfo(whiteinfo, *shaderTable, 1);

	ShaderTable::ShaderInfo blackinfo = {};
	blackinfo.heapIndices.push_back(blackHeapIndex);
	blackinfo.name = L"BoardTransHitGroup";
	addShaderInfo(blackinfo, *shaderTable, 1);
	blackinfo.name = L"BoardRefHitGroup";
	addShaderInfo(blackinfo, *shaderTable, 1);
	blackinfo.name = L"BoardHitGroup";
	addShaderInfo(blackinfo, *shaderTable, 1);
	
	mBLAS = AccelerationStructure::InitBottomLevelTriangleAS(device, cmdList, mGeoDescs.data(), mGeoDescs.size());

	CreateInstanceDesc(mInstanceDescs, hitGroupIndex);
}

void ChessBoard::CreateLine(std::vector<Vector3>& first, std::vector<Vector3>& second, const Square& base, unsigned int lineOffset)
{
	const unsigned int LINE_SQUARE_NUM = 8U;
	const unsigned int VERTNUM = 4U;
	const unsigned int indexOffset = LINE_SQUARE_NUM * VERTNUM * lineOffset / 2U;
	const float LENGTH = 3.0f;
	Square s = base;
	for (unsigned int i = 0; i < LINE_SQUARE_NUM / 2; ++i)
	{
		unsigned int index = i * VERTNUM + indexOffset;
		memcpy(&first[index], &s, sizeof(s));
		s.Add(Vector3(LENGTH, 0.0f, 0.0f));
		memcpy(&second[index], &s, sizeof(s));
		s.Add(Vector3(LENGTH, 0.0f, 0.0f));
	}
}

void ChessBoard::CreateBoard(std::vector<Vector3>& first, std::vector<Vector3>& second)
{
	const int LENGTH = 3.0f;
	Square s = {};
	s.pos1 = { 0.0f, 0.0f, 0.0f };
	s.pos2 = { LENGTH, 0.0f, 0.0f };
	s.pos3 = { LENGTH, 0.0f, LENGTH };
	s.pos4 = { 0.0f, 0.0f, LENGTH };

	//左奥に設定しなおす
	s.Add(Vector3(-LENGTH * 4, 0.0f, -LENGTH * 4));
	unsigned int lineNum = 8U;
	for (unsigned int i = 0; i < lineNum / 2U; ++i)
	{
		CreateLine(first, second, s, i * 2);
		s.Add(Vector3(0.0f, 0.0f, LENGTH));
		CreateLine(second, first, s, i * 2 + 1);
		s.Add(Vector3(0.0f, 0.0f, LENGTH));
	}
}

void ChessBoard::CreateIndices(std::vector<unsigned int>& indices)
{
	unsigned int base[6] = {
		0,1,2,
		0,2,3
	};

	unsigned int offset = 0;

	for (unsigned int i = 0; i < indices.size(); i += 6)
	{
		for (unsigned int j = 0; j < 6; ++j)
		{
			indices[i + j] = base[j] + offset;
		}
		offset += 4U;
	}
}

void ChessBoard::WriteVertexBuffer(MWCptr<ID3D12Resource>& vertexBuffer, std::vector<Vector3>& verts, const MWCptr<ID3D12Device5>& device)
{
	unsigned int bufferSize = sizeof(verts[0]) * verts.size();
	vertexBuffer = d3d_create_helper::CreateBuffer(device, bufferSize, D3D12_RESOURCE_FLAG_NONE, D3D12_RESOURCE_STATE_GENERIC_READ, d3d_create_helper::GetUploadHeapProps());
	
	void* data = nullptr;
	vertexBuffer->Map(0, nullptr, &data);

	memcpy(data, verts.data(), sizeof(verts[0]) * verts.size());

	vertexBuffer->Unmap(0, nullptr);
}

void ChessBoard::WriteIndexBuffer(MWCptr<ID3D12Resource>& indexBuffer, std::vector<unsigned int>& indices, const MWCptr<ID3D12Device5>& device)
{
	indexBuffer = d3d_create_helper::CreateBuffer(device, sizeof(indices[0]) * indices.size(), D3D12_RESOURCE_FLAG_NONE, D3D12_RESOURCE_STATE_GENERIC_READ, d3d_create_helper::GetUploadHeapProps());

	void* data = nullptr;
	indexBuffer->Map(0, nullptr, &data);

	memcpy(data, indices.data(), sizeof(indices[0]) * indices.size());

	indexBuffer->Unmap(0, nullptr);
}

D3D12_RAYTRACING_GEOMETRY_DESC ChessBoard::CreateTriangleGeometry(MWCptr<ID3D12Resource>& vertexBuffer, unsigned int vertexCount, MWCptr<ID3D12Resource> indexBuffer, unsigned int indexCount)
{
	AccelerationStructure::TriangleBuffers triangle;
	triangle.indexBuffer = indexBuffer;
	triangle.indexCount = indexCount;
	triangle.indexFormat = DXGI_FORMAT_R32_UINT;
	triangle.vertexBuffer = vertexBuffer;
	triangle.vertexCount = vertexCount;
	triangle.vertexFormat = DXGI_FORMAT_R32G32B32_FLOAT;
	triangle.vertexStride = sizeof(Vector3);

	return AccelerationStructure::CreateTriangleGeometryDesc(triangle, D3D12_RAYTRACING_GEOMETRY_FLAG_OPAQUE);
}

unsigned int ChessBoard::AddHeapColorCBV(MWCptr<ID3D12Resource>& resource, const Vector3& color, D3DDescriptorHeap& heap, unsigned int bufferSize)
{
	void* data = nullptr;
	resource->Map(0, nullptr, &data);
	memcpy(data, &color, sizeof(color));
	resource->Unmap(0, nullptr);
	D3D12_CONSTANT_BUFFER_VIEW_DESC cbv = {};
	cbv.BufferLocation = resource->GetGPUVirtualAddress();
	cbv.SizeInBytes = bufferSize;
	return heap.AddViewDesc(cbv);
}

