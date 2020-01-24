#pragma once
#include "GameObject.h"

class ChessBoard :
	public GameObject
{
public:
	ChessBoard(std::vector<D3D12_RAYTRACING_INSTANCE_DESC>& instanceDesc, const MWCptr<ID3D12Device5>& device,
		const MWCptr<ID3D12GraphicsCommandList4>& cmdList, D3DDescriptorHeap& heap, std::shared_ptr<ShaderTable>& shaderTable);
private:
	struct Square;

	void CreateChessBoard(const MWCptr<ID3D12Device5>& device, const MWCptr<ID3D12GraphicsCommandList4>& cmdList,
		D3DDescriptorHeap& heap, std::shared_ptr<ShaderTable>& shaderTable);

	void CreateLine(std::vector<Vector3>& first, std::vector<Vector3>& second, const Square& base, unsigned int lineOffset);
	void CreateBoard(std::vector<Vector3>& first, std::vector<Vector3>& second);
	void CreateIndices(std::vector<unsigned int>& indices);
	void WriteVertexBuffer(MWCptr<ID3D12Resource>& vertexBuffer, std::vector<Vector3>& verts, const MWCptr<ID3D12Device5>& device);
	void WriteIndexBuffer(MWCptr<ID3D12Resource>& indexBuffer, std::vector<unsigned int>& indices, const MWCptr<ID3D12Device5>& device);
	D3D12_RAYTRACING_GEOMETRY_DESC CreateTriangleGeometry(MWCptr<ID3D12Resource>& vertexBuffer, unsigned int vertexCount, MWCptr<ID3D12Resource> indexBuffer, unsigned int indexCount);
	unsigned int AddHeapColorCBV(MWCptr<ID3D12Resource>& resource, const Vector3& color, D3DDescriptorHeap& heap, unsigned int bufferSize);

	MWCptr<ID3D12Resource> mWhiteVertexBuffer;
	MWCptr<ID3D12Resource> mWhiteIndexBuffer;
	MWCptr<ID3D12Resource> mWhiteMaterial;//îíêF

	MWCptr<ID3D12Resource> mBlackVertexBuffer;
	MWCptr<ID3D12Resource> mBlackIndexBuffer;
	MWCptr<ID3D12Resource> mBlackMaterial;//çïêF

};