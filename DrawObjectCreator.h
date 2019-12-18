#pragma once
#include "AccelerationStructure/AccelerationStructure.h"
#include "Comptr.h"
#include <memory>
#include <string>

class GameObject;
class D3DDescriptorHeap;
class ShaderTable;
struct D3D12_RAYTRACING_INSTANCE_DESC;
struct ID3D12Device5;
struct ID3D12GraphicsCommandList4;

namespace DrawObject {
	using HitGroupPattern = std::vector<std::wstring>;
};
class DrawObjectCreator
{
public:
	DrawObjectCreator(D3DDescriptorHeap& heap,const MWCptr<ID3D12Device5>& device,
		const MWCptr<ID3D12GraphicsCommandList4>& commandList, const std::shared_ptr<ShaderTable>& shaderTable);
	std::shared_ptr<GameObject> LoadFMD(const std::string& path);
	void AddHitGroupPattern(const DrawObject::HitGroupPattern& pattern);
	AccelerationStructure::ASBuffer CreateTLAS();
	AccelerationStructure::ASBuffer GetTLAS() const;
	void UpdateTLAS();
private:
	std::vector<D3D12_RAYTRACING_INSTANCE_DESC> mInstanceDescs;
	AccelerationStructure::ASBuffer mTLAS;
	D3DDescriptorHeap& mCbvSrvUavHeap;
	MWCptr<ID3D12Device5> mDevice;
	MWCptr<ID3D12GraphicsCommandList4> mCommandList;
	std::shared_ptr<ShaderTable> mShaderTable;
	std::vector<DrawObject::HitGroupPattern> mHitGroupPattern;
};

