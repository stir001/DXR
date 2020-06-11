#include "Mathatic.h"
#include <cmath>
#include <string>

Vector2 Vector2::operator+(const Vector2& v)
{
	return Vector2(x + v.x, y + v.y);
}

Vector2 Vector2::operator+=(const Vector2& v)
{
	return *this = *this + v;
}

Vector2 Vector2::operator-(const Vector2& v)
{
	return Vector2(x - v.x, y - v.y);
}

Vector2 Vector2::operator-=(const Vector2& v)
{
	return *this = *this - v;
}

Vector3 Vector3::operator+(const Vector3& v) const {
	return Vector3(x + v.x, y + v.y, z + v.z);
}
Vector3 Vector3::operator-(const Vector3& v) const {
	return Vector3(x - v.x, y - v.y, z - v.z);
}
Vector3 Vector3::operator+=(const Vector3& v)
{
	return *this = *this + v;
}
Vector3 Vector3::operator-=(const Vector3& v)
{
	return *this = *this - v;
}
Vector3 Vector3::operator/(const float v) const
{
	return Vector3(x / v, y / v, z / v);
}
Vector3 Vector3::operator/=(const float v)
{
	return *this = *this / v;
}

float Vector3::Dot(const Vector3& lval, const Vector3& rval)
{
	return lval.x * rval.x + lval.y * rval.y + lval.z * rval.z;
}
Vector3 Vector3::Cross(const Vector3& lval, const Vector3& rval)
{
	return Vector3(lval.y * rval.z - lval.z * rval.y, lval.z * rval.x - lval.x * rval.z, lval.x * rval.y - lval.y * rval.x);
}

Vector3 operator*(const Vector3& vec, const float v)
{
	return Vector3(vec.x * v, vec.y * v, vec.z * v);
}

Vector3 operator*(const float v, const Vector3& vec)
{
	return Vector3(vec.x * v, vec.y * v, vec.z * v);
}

Vector4 Vector4::operator+=(const Vector3& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

Vector4 Vector4::operator=(const Vector3& v)
{
	x = v.x;
	y = v.y;
	z = v.z;
	return *this;
}

Vector4 Vector4::operator-(const Vector4& v)
{
	return Vector4(x - v.x, y - v.y, z - v.z, w);
}

Matrix Matrix::operator+(const Matrix& m) const
{
	Matrix ret = {
		mat[0][0] + m.mat[0][0], mat[0][1] + m.mat[0][1], mat[0][2] + m.mat[0][3],
		mat[1][0] + m.mat[1][0], mat[1][1] + m.mat[1][1], mat[1][2] + m.mat[1][3],
		mat[2][0] + m.mat[2][0], mat[2][1] + m.mat[2][1], mat[2][2] + m.mat[2][3],
		mat[3][0] + m.mat[3][0], mat[3][1] + m.mat[3][1], mat[3][2] + m.mat[3][3]
	};
	return ret;
}

Matrix Matrix::operator-(const Matrix& m) const
{
	Matrix ret = {
		mat[0][0] - m.mat[0][0], mat[0][1] - m.mat[0][1], mat[0][2] - m.mat[0][3],
		mat[1][0] - m.mat[1][0], mat[1][1] - m.mat[1][1], mat[1][2] - m.mat[1][3],
		mat[2][0] - m.mat[2][0], mat[2][1] - m.mat[2][1], mat[2][2] - m.mat[2][3],
		mat[3][0] - m.mat[3][0], mat[3][1] - m.mat[3][1], mat[3][2] - m.mat[3][3]
	};
	return ret;
}

Matrix Matrix::operator*(const Matrix& m) const
{
	auto& v = m.mat;
	Matrix ret = {
		mat[0][0] * v[0][0] + mat[0][1] * v[1][0] + mat[0][2] * v[2][0] + mat[0][3] * v[3][0], mat[0][0] * v[0][1] + mat[0][1] * v[1][1] + mat[0][2] * v[2][1] + mat[0][3] * v[3][1], mat[0][0] * v[0][2] + mat[0][1] * v[1][2] + mat[0][2] * v[2][2] + mat[0][3] * v[3][2], mat[0][0] * v[0][3] + mat[0][1] * v[1][3] + mat[0][2] * v[2][3] + mat[0][3] * v[3][3],
		mat[1][0] * v[0][0] + mat[1][1] * v[1][0] + mat[1][2] * v[2][0] + mat[1][3] * v[3][0], mat[1][0] * v[0][1] + mat[1][1] * v[1][1] + mat[1][2] * v[2][1] + mat[1][3] * v[3][1], mat[1][0] * v[0][2] + mat[1][1] * v[1][2] + mat[1][2] * v[2][2] + mat[1][3] * v[3][2], mat[1][0] * v[0][3] + mat[1][1] * v[1][3] + mat[1][2] * v[2][3] + mat[1][3] * v[3][3],
		mat[2][0] * v[0][0] + mat[2][1] * v[1][0] + mat[2][2] * v[2][0] + mat[2][3] * v[3][0], mat[2][0] * v[0][1] + mat[2][1] * v[1][1] + mat[2][2] * v[2][1] + mat[2][3] * v[3][1], mat[2][0] * v[0][2] + mat[2][1] * v[1][2] + mat[2][2] * v[2][2] + mat[2][3] * v[3][2], mat[2][0] * v[0][3] + mat[2][1] * v[1][3] + mat[2][2] * v[2][3] + mat[2][3] * v[3][3],
		mat[3][0] * v[0][0] + mat[3][1] * v[1][0] + mat[3][2] * v[2][0] + mat[3][3] * v[3][0], mat[3][0] * v[0][1] + mat[3][1] * v[1][1] + mat[3][2] * v[2][1] + mat[3][3] * v[3][1], mat[3][0] * v[0][2] + mat[3][1] * v[1][2] + mat[3][2] * v[2][2] + mat[3][3] * v[3][2], mat[3][0] * v[0][3] + mat[3][1] * v[1][3] + mat[3][2] * v[2][3] + mat[3][3] * v[3][3],
	};
	return ret;
}

Matrix Matrix::operator*=(const Matrix& m)
{
	return *this = *this * m;
}

Matrix Matrix::operator/(const float v) const
{
	Matrix ret = {
		mat[0][0] / v, mat[0][1] / v, mat[0][2] / v, mat[0][3] / v,
		mat[1][0] / v, mat[1][1] / v, mat[1][2] / v, mat[1][3] / v,
		mat[2][0] / v, mat[2][1] / v, mat[2][2] / v, mat[2][3] / v,
		mat[3][0] / v, mat[3][1] / v, mat[3][2] / v, mat[3][3] / v,
	};
	return ret;
}

Vector4 Matrix::operator*(const Vector4& v) const
{
	Vector4 ret = {
		_00 * v.x + _01 * v.y + _02 * v.z + _03 * v.w,
		_10 * v.x + _11 * v.y + _12 * v.z + _13 * v.w,
		_20 * v.x + _21 * v.y + _22 * v.z + _23 * v.w,
		_30 * v.x + _31 * v.y + _32 * v.z + _33 * v.w
	};
	return ret;
}

Vector4 Matrix::operator*(const Vector3& v) const
{
	Vector4 ret = {
		_00 * v.x + _01 * v.y + _02 * v.z + _03 * 1.0f,
		_10 * v.x + _11 * v.y + _12 * v.z + _13 * 1.0f,
		_20 * v.x + _21 * v.y + _22 * v.z + _23 * 1.0f,
		_30 * v.x + _31 * v.y + _32 * v.z + _33 * 1.0f
	};
	return ret;
}

bool Matrix::operator ==(const Matrix& mat) const
{
	return	_00 == mat._00 && _01 == mat._01 && _02 == mat._02 && _03 == mat._03 &&
		_10 == mat._10 && _11 == mat._11 && _12 == mat._12 && _13 == mat._13 &&
		_20 == mat._20 && _21 == mat._21 && _22 == mat._22 && _23 == mat._23 &&
		_30 == mat._30 && _31 == mat._31 && _32 == mat._32 && _33 == mat._33;
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

Int2 Int2::operator-() const
{
	return Int2(-x, -y);
}

Int2 Int2::operator+(const Int2& val) const
{
	return Int2(x + val.x, y + val.y);
}

Int2 Int2::operator-(const Int2& val) const
{
	return Int2(x - val.x, y - val.y);
}

Int2 Int2::operator+=(const Int2& val)
{
	x += val.x;
	y += val.y;
	return *this;
}

Int2 Int2::operator-=(const Int2& val)
{
	return *this += -val;
}

bool Int2::operator==(const Int2& val) const
{
	return x == val.x && y == val.y;
}
