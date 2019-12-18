#include "Camera.h"
#include "D3DCreateHelper.h"
#include "D3DDescriptorHeap.h"
#include <d3d12.h>
#include <DirectXMath.h>

Camera::Camera(const MWCptr<ID3D12Device5>& device, D3DDescriptorHeap& heap, float aspectRatio) : mBuffer{ Matrix::Identity(),Vector4()}, mFov(DirectX::XM_PIDIV2)
	, mNear(1.0f), mFar(100000.0f), mRota(0.0f, 0.0f, 0.0f), mView(Matrix::Identity()), mProjection(Matrix::Identity()), mAspectratio(aspectRatio), mHeapIndex(UINT_MAX)
	, mDir{0.0f,0.0f,1.0f}, mRotaMatrix(Matrix::Identity())
{
	Init(device, heap);
}

void Camera::SetPos(const Vector3& pos)
{
	mBuffer.pos = pos;
	CalculateViewProj();
	Map();
}

void Camera::AddPos(const Vector3& val)
{
	mBuffer.pos += val;
	CalculateViewProj();
	Map();
}

void Camera::SetDir(const Vector3& dir)
{
	/*mDir = dir;
	CalculateViewProj();
	Map();*/
}

void Camera::AddRotaYAxis(const float rad)
{
	auto mat = DirectX::XMMatrixRotationY(rad);
	Matrix yRota;
	yRota = mat;
	mDir = yRota * Vector4(mDir.x, mDir.y, mDir.z, 1.0f);
	mRota.y += rad;
	mRotaMatrix *= Matrix::Transpose(yRota);
}

void Camera::AddRotaSideAxis(const float rad)
{
	auto cross = Vector3::Cross(mDir, {0.0f, 1.0f, 0.0f}).Normalized();
}

unsigned int Camera::GetCameraHeapIndex() const
{
	return mHeapIndex;
}

void Camera::Init(const MWCptr<ID3D12Device5>& device, D3DDescriptorHeap& heap)
{
	auto bufferSize = d3d_create_helper::Alignment(256, sizeof(mBuffer));
	mResource = d3d_create_helper::CreateBuffer(device, bufferSize,
		D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ,
		d3d_create_helper::GetUploadHeapProps());

	CalculateViewProj();
	Map();

	auto desc = d3d_create_helper::CreateConstantBufferView(mResource->GetGPUVirtualAddress(), bufferSize);
	mHeapIndex = heap.AddViewDesc(desc);
}

void Camera::CalculateViewProj()
{
	auto projection = DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV2, mAspectratio, mNear, mFar);
	
	auto pos = mBuffer.pos;
	auto translate = DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
	//auto rota = DirectX::XMMatrixRotationRollPitchYaw(mRota.x, mRota.y, mRota.z);
	DirectX::XMMATRIX rota;
	memcpy(&rota, &mRotaMatrix, sizeof(rota));
	auto afin = rota * translate;
	auto view = DirectX::XMMatrixInverse(nullptr, afin);
	mBuffer.inverseViewProj = DirectX::XMMatrixInverse(nullptr, view * projection);

	Vector3 dir;
	dir = mBuffer.inverseViewProj * Vector4(0.0f, 0.0f, 0.0f, 1.0f) - pos;
	dir.Normalized();
}

void Camera::Map()
{
	CameraConstntBuffer* map = nullptr;
	mResource->Map(0, nullptr, reinterpret_cast<void**>(&map));
	*map = mBuffer;
	mResource->Unmap(0, nullptr);
}
