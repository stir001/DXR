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
	GameObject(std::vector<D3D12_RAYTRACING_INSTANCE_DESC>& instanceDescs);
	const AccelerationStructure::ASBuffer& GetBLAS() const;
	virtual void SetPos(const Vector3& pos);
	virtual void AddPos(const Vector3& val);
	virtual void AddRotaY(const float rad);
	virtual void AddRotaZ(const float rad);
protected:
	virtual void UpdateMatrix();
	virtual void CreateInstanceDesc(std::vector<D3D12_RAYTRACING_INSTANCE_DESC>& instanceDescs, unsigned int hitGroupIndex);

	AccelerationStructure::ASBuffer mBLAS;
	std::vector<D3D12_RAYTRACING_GEOMETRY_DESC> mGeoDescs;//頂点情報をマテリアル単位で管理
	std::vector<D3D12_RAYTRACING_INSTANCE_DESC>& mInstanceDescs;
	unsigned int mInstanceDescIndex;
	Matrix mMat;
};

