#pragma once
#include "GameObject.h"

struct D3D12_SHADER_RESOURCE_VIEW_DESC;
struct D3D12_CONSTANT_BUFFER_VIEW_DESC;
struct Matrix;

class FmdGameObject :
	public GameObject
{
public: 
	FmdGameObject(const Fmd::FMDFileData& data, std::vector<D3D12_RAYTRACING_INSTANCE_DESC>& instanceDescs, const MWCptr<ID3D12Device5>& device,
		const MWCptr<ID3D12GraphicsCommandList4>& cmdList, D3DDescriptorHeap& heap, std::shared_ptr<ShaderTable>& shaderTable, const Vector4& color);
	FmdGameObject(const Fmd::FMDFileData& data, std::vector<D3D12_RAYTRACING_INSTANCE_DESC>& instanceDescs, const MWCptr<ID3D12Device5>& device,
		const MWCptr<ID3D12GraphicsCommandList4>& cmdList, D3DDescriptorHeap& heap, std::shared_ptr<ShaderTable>& shaderTable, const Vector4& color, const Matrix& initMatrix);
private:
	void InitFmd(const Fmd::FMDFileData& data, const MWCptr<ID3D12Device5>& device, const MWCptr<ID3D12GraphicsCommandList4>& cmdList,
		D3DDescriptorHeap& heap, std::shared_ptr<ShaderTable>& shaderTable, const Matrix& initMatrix);
	void InitVertices(std::vector<Vector3>& vertices, const Matrix& mat);
	void InitNormals(std::vector<Vector3>& normals, const Matrix& mat);
	unsigned int AddTransHitGroups(std::shared_ptr<ShaderTable>& shaderTable, unsigned int heapIndex, unsigned int addCount);


	MWCptr<ID3D12Resource> mVertexBuffer;
	MWCptr<ID3D12Resource> mIndexBuffer;
	MWCptr<ID3D12Resource> mNormalBuffer;
	MWCptr<ID3D12Resource> mMaterialBuffer;
	Vector4 mColor;
};

