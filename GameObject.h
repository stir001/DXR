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
	std::vector<D3D12_RAYTRACING_GEOMETRY_DESC> mGeoDescs;//���_�����}�e���A���P�ʂŊǗ�
	std::vector<D3D12_RAYTRACING_INSTANCE_DESC>& mInstanceDescs;//�A�t�B���s���Shader���Ŏg�p����HitGroup�̃I�t�Z�b�g�l���Ǘ�
	unsigned int mHitGroupIndex;
	D3DDescriptorHeap& mCbvSrvUavHeap;
};

