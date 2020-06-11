#pragma once
#include "Comptr.h"
#include "Mathatic.h"

class D3DDescriptorHeap;
struct ID3D12Resource;
struct ID3D12Device5;

class Camera
{
public:
	Camera(const MWCptr<ID3D12Device5>& device, D3DDescriptorHeap& heap, float aspectRatio);
	void SetPos(const Vector3& pos);
	void AddPos(const Vector3& val);
	void SetDir(const Vector3& dir);
	void AddRotaYAxis(const float rad);//Y����]
	void AddRotaSideAxis(const float rad);//������](����������Y�������ɐ����ȃx�N�g���ł̉�])
	unsigned int GetCameraHeapIndex() const;
	void MoveFront(float val);
	void MoveSide(float val);
	Vector3 Direction(const Vector2& screenPos);//�X�N���[�����W����������v�Z����
	Vector3 GetPos() const;
private:
	void Init(const MWCptr<ID3D12Device5>& device, D3DDescriptorHeap& heap);
	void CalculateViewProj();
	void Map();

	MWCptr<ID3D12Resource> mResource;

	struct CameraConstntBuffer
	{
		Matrix inverseViewProj;
		Vector4 pos;
	};

	CameraConstntBuffer mBuffer;

	Matrix mView;
	Matrix mProjection;
	Matrix mRotaMatrix;
	Vector3 mDir;
	float mFov;
	float mAspectratio;
	float mNear;
	float mFar;
	unsigned int mHeapIndex;
};

