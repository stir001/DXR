#include "Camera.h"
#include "D3DCreateHelper.h"
#include "D3DDescriptorHeap.h"
#include <d3d12.h>
#include <DirectXMath.h>

Camera::Camera(const MWCptr<ID3D12Device5>& device, D3DDescriptorHeap& heap, float aspectRatio) : mBuffer{ Matrix::Identity(),Vector4()}, mFov(DirectX::XM_PIDIV2)
	, mNear(1.0f), mFar(100000.0f), mView(Matrix::Identity()), mProjection(Matrix::Identity()), mAspectratio(aspectRatio), mHeapIndex(UINT_MAX)
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
	mRotaMatrix *= Matrix::Transpose(yRota);
	CalculateViewProj();
	Map();
}

void Camera::AddRotaSideAxis(const float rad)
{
	auto cross = Vector3::Cross(mDir, {0.0f, 1.0f, 0.0f}).Normalized();
	auto mat = DirectX::XMMatrixRotationAxis({ cross.x, cross.y, cross.z }, rad);
	Matrix sideRota;
	sideRota = mat;
	mRotaMatrix *= Matrix::Transpose(sideRota);
	CalculateViewProj();
	Map();
}

unsigned int Camera::GetCameraHeapIndex() const
{
	return mHeapIndex;
}

void Camera::MoveFront(float val)
{
	AddPos(Vector3(mDir.x, 0.0f, mDir.z) * val);
}

void Camera::MoveSide(float val)
{
	auto side = Vector3::Cross(mDir, Vector3(0.0f, 1.0f, 0.0f)).Normalized();
	AddPos(side * val);
}

Vector3 Camera::Direction(const Vector2& screenPos)
{
	auto pos = mBuffer.inverseViewProj * Vector4(screenPos.x, screenPos.y, 1.0f, 1.0f);
	return Vector3::Normalize(Vector3(pos.x, pos.y, pos.z) - Vector3(mBuffer.pos.x, mBuffer.pos.y, mBuffer.pos.z));
}

Vector3 Camera::GetPos() const
{
	return Vector3(mBuffer.pos.x, mBuffer.pos.y, mBuffer.pos.z);
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
	auto projection = DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV4, mAspectratio, mNear, mFar);
	
	auto pos = mBuffer.pos;
	auto translate = DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
	DirectX::XMMATRIX rota;
	memcpy(&rota, &mRotaMatrix, sizeof(rota));
	auto afin = rota * translate;
	auto view = DirectX::XMMatrixInverse(nullptr, afin);
	mBuffer.inverseViewProj = DirectX::XMMatrixInverse(nullptr, view * projection);
	//mBuffer.inverseViewProj = Matrix::Transpose(mBuffer.inverseViewProj);
}

void Camera::Map()
{
	CameraConstntBuffer* map = nullptr;
	mResource->Map(0, nullptr, reinterpret_cast<void**>(&map));
	*map = mBuffer;
	mResource->Unmap(0, nullptr);
}
