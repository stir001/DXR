#include "GameObject.h"
#include "D3DCreateHelper.h"
#include "FMDdata.h"
#include <d3d12.h>
#include <DirectXMath.h>


GameObject::GameObject(std::vector<D3D12_RAYTRACING_INSTANCE_DESC>& instanceDescs) 
	: mMat(Matrix::Identity()), mInstanceDescs(instanceDescs), mInstanceDescIndex(0), mBLAS()
{
}

const AccelerationStructure::ASBuffer& GameObject::GetBLAS() const
{
	return mBLAS;
}

void GameObject::SetPos(const Vector3& pos)
{
	mMat._03 = pos.x;
	mMat._13 = pos.y;
	mMat._23 = pos.z;
	UpdateMatrix();
}

void GameObject::AddPos(const Vector3& val)
{
	mMat *= Matrix::Translate(val);
	UpdateMatrix();
}

void GameObject::AddRotaY(const float rad)
{
	auto mat = DirectX::XMMatrixRotationY(rad);
	Matrix m;
	m = mat;
	mMat *= Matrix::Transpose(m);
	UpdateMatrix();
}

void GameObject::AddRotaZ(const float rad)
{
	auto mat = DirectX::XMMatrixRotationZ(rad);
	Matrix m;
	m = mat;
	mMat *= Matrix::Transpose(m);
	UpdateMatrix();
}

void GameObject::UpdateMatrix()
{
	float mat[3][4];
	memcpy(mat, &mMat, sizeof(float) * 3 * 4);
	memcpy(mInstanceDescs[mInstanceDescIndex].Transform, mat, sizeof(mat));
}

void GameObject::CreateInstanceDesc(std::vector<D3D12_RAYTRACING_INSTANCE_DESC>& instanceDescs, unsigned int hitGroupIndex)
{
	float mat[3][4];
	memcpy(mat, &mMat, sizeof(float) * 3 * 4);

	D3D12_RAYTRACING_INSTANCE_DESC instanceDesc;

	instanceDesc.AccelerationStructure = mBLAS.result->GetGPUVirtualAddress();
	instanceDesc.Flags = D3D12_RAYTRACING_INSTANCE_FLAGS::D3D12_RAYTRACING_INSTANCE_FLAG_NONE;
	instanceDesc.InstanceContributionToHitGroupIndex = hitGroupIndex;
	instanceDesc.InstanceID = instanceDescs.size();
	instanceDesc.InstanceMask = 0xff;
	memcpy(instanceDesc.Transform, mat, sizeof(mat));

	mInstanceDescIndex = instanceDescs.size();
	instanceDescs.push_back(instanceDesc);
}
