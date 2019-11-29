#pragma once
#include "Comptr.h"
#include "AccelerationStructure/AccelerationStructure.h"
#include <memory>
#include <vector>

class AccelerationStructure;
class D3DDescriptorHeap;
struct ID3D12GraphicsCommandList4;
struct ID3D12Resource;
struct ID3D12Device5;
struct D3D12_RAYTRACING_GEOMETRY_DESC;
struct D3D12_RAYTRACING_INSTANCE_DESC;

class GameObject
{
public:
	GameObject(std::vector<D3D12_RAYTRACING_INSTANCE_DESC>& instanceDescs, const MWCptr<ID3D12Device5>& device, const MWCptr<ID3D12GraphicsCommandList4>& cmdList, 
		D3DDescriptorHeap& heap, unsigned int hitGroupIndex);
	const AccelerationStructure::ASBuffer& GetBLAS() const;
	void Draw();
private:
	void InitTriangle();
	void CreateInstanceDesc();

	AccelerationStructure::ASBuffer mBLAS;
	MWCptr<ID3D12Resource> mVertexBuffer;
	MWCptr<ID3D12Device5> mDevice;
	MWCptr<ID3D12GraphicsCommandList4> mCmdList;
	std::vector<D3D12_RAYTRACING_GEOMETRY_DESC> mGeoDescs;//頂点情報をマテリアル単位で管理
	std::vector<D3D12_RAYTRACING_INSTANCE_DESC>& mInstanceDescs;//アフィン行列やShader内で使用するHitGroupのオフセット値を管理
	unsigned int mHitGroupIndex;
	D3DDescriptorHeap& mCbvSrvUavHeap;
};

