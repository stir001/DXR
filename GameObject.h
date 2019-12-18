#pragma once
#include "Comptr.h"
#include "AccelerationStructure/AccelerationStructure.h"
#include "Mathatic.h"
#include <memory>
#include <vector>

class D3DDescriptorHeap;
class ShaderTable;
struct ID3D12GraphicsCommandList4;
struct ID3D12Resource;
struct ID3D12Device5;
struct D3D12_RAYTRACING_GEOMETRY_DESC;
struct D3D12_RAYTRACING_INSTANCE_DESC;

namespace Fmd {
	struct FMDFileData;
};

class GameObject
{
public:
	GameObject(const Fmd::FMDFileData& data, std::vector<D3D12_RAYTRACING_INSTANCE_DESC>& instanceDescs, const MWCptr<ID3D12Device5>& device,
		const MWCptr<ID3D12GraphicsCommandList4>& cmdList, D3DDescriptorHeap& heap, std::shared_ptr<ShaderTable>& shaderTable);
	const AccelerationStructure::ASBuffer& GetBLAS() const;
	void SetPos(const Vector3& pos);
	void AddPos(const Vector3& val);

	void Draw();
private:
	void InitFmd(const Fmd::FMDFileData& data, const MWCptr<ID3D12Device5>& device, const MWCptr<ID3D12GraphicsCommandList4>& cmdList,
		D3DDescriptorHeap& heap, std::shared_ptr<ShaderTable>& shaderTable);
	void CreateInstanceDesc(std::vector<D3D12_RAYTRACING_INSTANCE_DESC>& instanceDescs, unsigned int hitGroupIndex);
	void UpdateMatrix();

	AccelerationStructure::ASBuffer mBLAS;
	MWCptr<ID3D12Resource> mVertexBuffer;
	MWCptr<ID3D12Resource> mIndexBuffer;
	MWCptr<ID3D12Resource> mNormalBuffer;
	std::vector<D3D12_RAYTRACING_GEOMETRY_DESC> mGeoDescs;//頂点情報をマテリアル単位で管理
	std::vector<D3D12_RAYTRACING_INSTANCE_DESC>& mInstanceDescs;
	unsigned int mInstansDescIndex;
	Matrix mMat;
};

