#pragma once

namespace DirectX {
	struct XMMATRIX;
}

struct Vector4;
struct Vector3;
struct Vector2;

struct Vector2 {
	Vector2() : x(0), y(0){};
	Vector2(float x, float y) : x(x), y(y) {};

	float x;
	float y;

	Vector2 operator+(const Vector2& v)
	{
		return Vector2(x + v.x, y + v.y);
	}

	Vector2 operator+=(const Vector2& v)
	{
		return *this = *this + v;
	}

	Vector2 operator-(const Vector2& v)
	{
		return Vector2(x - v.x, y - v.y);
	}

	Vector2 operator-=(const Vector2& v)
	{
		return *this = *this - v;
	}
};

struct Vector3
{
	Vector3() : x(0), y(0), z(0) {};
	Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
	float x;
	float y;
	float z;
	Vector3 operator+(const Vector3& v) const {
		return Vector3(x + v.x, y + v.y, z + v.z);
	}
	Vector3 operator-(const Vector3& v) const {
		return Vector3(x - v.x, y - v.y, z - v.z);
	}
	Vector3 operator+=(const Vector3& v)
	{
		return *this = *this + v;
	}
	Vector3 operator-=(const Vector3& v)
	{
		return *this = *this - v;
	}
	Vector3 operator/(const float v) const
	{
		return Vector3(x / v, y / v, z / v);
	}
	Vector3 operator/=(const float v)
	{
		return *this = *this / v;
	}
	Vector3 operator=(const Vector4& v);

	Vector3 Normalized();

	static float Dot(const Vector3& lval, const Vector3& rval)
	{
		return lval.x * rval.x + lval.y * rval.y + lval.z * rval.z;
	}

	static Vector3 Cross(const Vector3& lval, const Vector3& rval)
	{
		return Vector3(lval.y * rval.z - lval.z * rval.y, lval.z * rval.x - lval.x * rval.z, lval.x * rval.y - lval.y * rval.x);
	}

	static Vector3 Normalize(const Vector3& v);

	static float Length(const Vector3& v);
};

Vector3 operator*(const Vector3& vec, const float v);

Vector3 operator*(const float v, const Vector3& vec);

struct Vector4
{
	Vector4() : x(0), y(0), z(0), w(1.0f) {};
	Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {};
	float x;
	float y;
	float z;
	float w;

	Vector4 operator+=(const Vector3& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}

	Vector4 operator=(const Vector3& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
		return *this;
	}

	Vector4 operator-(const Vector4& v)
	{
		return Vector4(x - v.x, y - v.y, z - v.z, w);
	}
};

struct Matrix {
	union {
		float mat[4][4];
		struct {
			float _00, _01, _02, _03;
			float _10, _11, _12, _13;
			float _20, _21, _22, _23;
			float _30, _31, _32, _33;
		};
	};

	Matrix operator+(const Matrix& m)
	{
		Matrix ret = {	 mat[0][0] + m.mat[0][0], mat[0][1] + m.mat[0][1], mat[0][2] + m.mat[0][3]
						,mat[1][0] + m.mat[1][0], mat[1][1] + m.mat[1][1], mat[1][2] + m.mat[1][3]
						,mat[2][0] + m.mat[2][0], mat[2][1] + m.mat[2][1], mat[2][2] + m.mat[2][3]
						,mat[3][0] + m.mat[3][0], mat[3][1] + m.mat[3][1], mat[3][2] + m.mat[3][3]
		};
		return ret;
	}

	Matrix operator-(const Matrix& m)
	{
		Matrix ret = {	 mat[0][0] - m.mat[0][0], mat[0][1] - m.mat[0][1], mat[0][2] - m.mat[0][3]
						,mat[1][0] - m.mat[1][0], mat[1][1] - m.mat[1][1], mat[1][2] - m.mat[1][3]
						,mat[2][0] - m.mat[2][0], mat[2][1] - m.mat[2][1], mat[2][2] - m.mat[2][3]
						,mat[3][0] - m.mat[3][0], mat[3][1] - m.mat[3][1], mat[3][2] - m.mat[3][3]
		};
		return ret;
	}

	Matrix operator*(const Matrix& m)
	{
		auto& v = m.mat;
		Matrix ret = {	mat[0][0] * v[0][0] + mat[0][1] * v[1][0] + mat[0][2] * v[2][0] + mat[0][3] * v[3][0], mat[0][0] * v[0][1] + mat[0][1] * v[1][1] + mat[0][2] * v[2][1] + mat[0][3] * v[3][1], mat[0][0] * v[0][2] + mat[0][1] * v[1][2] + mat[0][2] * v[2][2] + mat[0][3] * v[3][2], mat[0][0] * v[0][3] + mat[0][1] * v[1][3] + mat[0][2] * v[2][3] + mat[0][3] * v[3][3],
						mat[1][0] * v[0][0] + mat[1][1] * v[1][0] + mat[1][2] * v[2][0] + mat[1][3] * v[3][0], mat[1][0] * v[0][1] + mat[1][1] * v[1][1] + mat[1][2] * v[2][1] + mat[1][3] * v[3][1], mat[1][0] * v[0][2] + mat[1][1] * v[1][2] + mat[1][2] * v[2][2] + mat[1][3] * v[3][2], mat[1][0] * v[0][3] + mat[1][1] * v[1][3] + mat[1][2] * v[2][3] + mat[1][3] * v[3][3],
						mat[2][0] * v[0][0] + mat[2][1] * v[1][0] + mat[2][2] * v[2][0] + mat[2][3] * v[3][0], mat[2][0] * v[0][1] + mat[2][1] * v[1][1] + mat[2][2] * v[2][1] + mat[2][3] * v[3][1], mat[2][0] * v[0][2] + mat[2][1] * v[1][2] + mat[2][2] * v[2][2] + mat[2][3] * v[3][2], mat[2][0] * v[0][3] + mat[2][1] * v[1][3] + mat[2][2] * v[2][3] + mat[2][3] * v[3][3],
						mat[3][0] * v[0][0] + mat[3][1] * v[1][0] + mat[3][2] * v[2][0] + mat[3][3] * v[3][0], mat[3][0] * v[0][1] + mat[3][1] * v[1][1] + mat[3][2] * v[2][1] + mat[3][3] * v[3][1], mat[3][0] * v[0][2] + mat[3][1] * v[1][2] + mat[3][2] * v[2][2] + mat[3][3] * v[3][2], mat[3][0] * v[0][3] + mat[3][1] * v[1][3] + mat[3][2] * v[2][3] + mat[3][3] * v[3][3],
		};
		return ret;
	}

	Matrix operator*=(const Matrix& m)
	{
		return *this = *this * m;
	}

	Matrix operator/(const float v)
	{
		Matrix ret = {
			mat[0][0] / v, mat[0][1] / v, mat[0][2] / v, mat[0][3] / v,
			mat[1][0] / v, mat[1][1] / v, mat[1][2] / v, mat[1][3] / v,
			mat[2][0] / v, mat[2][1] / v, mat[2][2] / v, mat[2][3] / v,
			mat[3][0] / v, mat[3][1] / v, mat[3][2] / v, mat[3][3] / v,
		};
		return ret;
	}

	Matrix operator=(const DirectX::XMMATRIX& mat);

	Vector4 operator*(const Vector4& v)
	{
		Vector4 ret = {
			_00 * v.x + _01 * v.y + _02 * v.z + _03 * v.w,
			_10 * v.x + _11 * v.y + _12 * v.z + _13 * v.w,
			_20 * v.x + _21 * v.y + _22 * v.z + _23 * v.w,
			_30 * v.x + _31 * v.y + _32 * v.z + _33 * v.w
		};
		return ret;
	}

	static Matrix Identity();
	static Matrix Translate(const Vector3& offset);
	static Matrix Rotate();
	static Matrix Transpose(const Matrix& mat);
};

Matrix operator*(const Matrix& m, const float v);
Matrix operator*(const float v, const Matrix& m);
