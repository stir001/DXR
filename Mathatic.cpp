#include "Mathatic.h"
#include <cmath>
#include <string>

Vector3 operator*(const Vector3& vec, const float v)
{
	return Vector3(vec.x * v, vec.y * v, vec.z * v);
}

Vector3 operator*(const float v, const Vector3& vec)
{
	return Vector3(vec.x * v, vec.y * v, vec.z * v);
}

Matrix operator*(const Matrix& m, const float v)
{
	Matrix ret = {
		m.mat[0][0] * v, m.mat[0][1] * v, m.mat[0][2] * v, m.mat[0][3] * v,
		m.mat[1][0] * v, m.mat[1][1] * v, m.mat[1][2] * v, m.mat[1][3] * v,
		m.mat[2][0] * v, m.mat[2][1] * v, m.mat[2][2] * v, m.mat[2][3] * v,
		m.mat[3][0] * v, m.mat[3][1] * v, m.mat[3][2] * v, m.mat[3][3] * v,
	};
	return ret;
}

Matrix operator*(const float v, const Matrix& m)
{
	return m * v;
}

Vector3 Vector3::operator-() const
{
	return Vector3(-x, -y, -z);
}

Vector3 Vector3::operator=(const Vector4& v)
{
	x = v.x;
	y = v.y;
	z = v.z;
	return *this;
}

Vector3 Vector3::Normalized()
{
	return *this = Normalize(*this);
}

Vector3 Vector3::Normalize(const Vector3& v)
{
	auto length = Vector3::Length(v);
	return v / length;
}

float Vector3::Length(const Vector3& v)
{
	return sqrtf(Vector3::Dot(v,v));
}

Matrix Matrix::operator=(const DirectX::XMMATRIX& mat)
{
	memcpy(this, &mat, sizeof(*this));
	return *this;
}

Matrix Matrix::Identity()
{
	Matrix ret = {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
	};
	return ret;
}

Matrix Matrix::Translate(const Vector3& offset)
{
	auto ret = Identity();
	ret._03 = offset.x;
	ret._13 = offset.y;
	ret._23 = offset.z;
	return ret;
}

Matrix Matrix::Rotate()
{
	return Matrix();
}

Matrix Matrix::Transpose(const Matrix& mat)
{
	Matrix ret = {
		mat._00, mat._10, mat._20, mat._30,
		mat._01, mat._11, mat._21, mat._31,
		mat._02, mat._12, mat._22, mat._32,
		mat._03, mat._13, mat._23, mat._33
	};
	return ret;
}
