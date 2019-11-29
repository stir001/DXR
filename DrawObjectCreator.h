#pragma once
#include "AccelerationStructure/AccelerationStructure.h"
#include "Comptr.h"
#include <memory>
#include <string>

class GameObject;
class D3DDescriptorHeap;
struct D3D12_RAYTRACING_INSTANCE_DESC;
struct ID3D12Device5;
struct ID3D12GraphicsCommandList4;

class DrawObjectCreator
{
public:
	DrawObjectCreator(D3DDescriptorHeap& heap, const MWCptr<ID3D12Device5>& device,
		const MWCptr<ID3D12GraphicsCommandList4>& commandList);
	std::shared_ptr<GameObject> CreateGameObject(unsigned int hitGroupIndex);
	std::shared_ptr<GameObject> LoadFMD(const std::string& path);
	AccelerationStructure::ASBuffer CreateTLAS();
	AccelerationStructure::ASBuffer GetTLAS() const;
private:
	std::vector<D3D12_RAYTRACING_INSTANCE_DESC> mInstanceDescs;
	AccelerationStructure::ASBuffer mTLAS;
	D3DDescriptorHeap& mCbvSrvUavHeap;
	MWCptr<ID3D12Device5> mDevice;
	MWCptr<ID3D12GraphicsCommandList4> mCommandList;
};

