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
struct Vector4;
struct Matrix;

namespace DrawObject {
	using HitGroupPattern = std::vector<std::wstring>;
};
class DrawObjectCreator
{
public:
	DrawObjectCreator(D3DDescriptorHeap& heap,const MWCptr<ID3D12Device5>& device,
		const MWCptr<ID3D12GraphicsCommandList4>& commandList, const std::shared_ptr<ShaderTable>& shaderTable);
	std::shared_ptr<GameObject> LoadChess(const std::string& path, const Vector4& color);
	std::shared_ptr<GameObject> LoadChess(const std::string& path, const Vector4& color, const Matrix& initMat);
	std::shared_ptr<GameObject> CreateChessBoard();
	void AddHitGroupPattern(const DrawObject::HitGroupPattern& pattern);
	AccelerationStructure::ASBuffer CreateTLAS();
	AccelerationStructure::ASBuffer GetTLAS() const;
	void UpdateTLAS();
	unsigned int GetTLASHeapIndex() const;
	void SetTLAS();
private:
	std::vector<D3D12_RAYTRACING_INSTANCE_DESC> mInstanceDescs;
	AccelerationStructure::ASBuffer mTLAS;
	D3DDescriptorHeap& mCbvSrvUavHeap;
	MWCptr<ID3D12Device5> mDevice;
	MWCptr<ID3D12GraphicsCommandList4> mCommandList;
	std::shared_ptr<ShaderTable> mShaderTable;
	std::vector<DrawObject::HitGroupPattern> mHitGroupPattern;
	unsigned int mTLASHeapIndex;
};

