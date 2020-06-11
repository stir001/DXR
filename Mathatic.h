#pragma once

const float PI = 3.14159265f;

namespace DirectX {
	struct XMMATRIX;
}

struct Vector4;
struct Vector3;
struct Vector2;

struct Vector2 {
	Vector2() : x(0), y(0){};
	Vector2(float x, float y) : x(x), y(y) {};

	float x, y;

	Vector2 operator+(const Vector2& v);
	Vector2 operator+=(const Vector2& v);
	Vector2 operator-(const Vector2& v);
	Vector2 operator-=(const Vector2& v);
};

struct Vector3
{
	Vector3() : x(0), y(0), z(0) {};
	Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
	float x, y, z;

	Vector3 operator-() const;
	Vector3 operator+(const Vector3& v) const;
	Vector3 operator-(const Vector3& v) const;
	Vector3 operator+=(const Vector3& v);
	Vector3 operator-=(const Vector3& v);
	Vector3 operator/(const float v) const;
	Vector3 operator/=(const float v);
	Vector3 operator=(const Vector4& v);

	Vector3 Normalized();

	static float Dot(const Vector3& lval, const Vector3& rval);
	static Vector3 Cross(const Vector3& lval, const Vector3& rval);
	static Vector3 Normalize(const Vector3& v);
	static float Length(const Vector3& v);
};

Vector3 operator*(const Vector3& vec, const float v);

Vector3 operator*(const float v, const Vector3& vec);

struct Vector4
{
	Vector4() : x(0), y(0), z(0), w(1.0f) {};
	Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {};
	float x, y, z, w;

	Vector4 operator+=(const Vector3& v);
	Vector4 operator=(const Vector3& v);
	Vector4 operator-(const Vector4& v);
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

	Matrix operator+(const Matrix& m) const;
	Matrix operator-(const Matrix& m) const;
	Matrix operator*(const Matrix& m) const;
	Matrix operator*=(const Matrix& m);
	Matrix operator/(const float v) const;
	Matrix operator=(const DirectX::XMMATRIX& mat);
	Vector4 operator*(const Vector4& v) const;
	Vector4 operator*(const Vector3& v) const;
	bool operator ==(const Matrix& mat) const;

	static Matrix Identity();
	static Matrix Translate(const Vector3& offset);
	static Matrix Rotate();
	static Matrix Transpose(const Matrix& mat);
};

Matrix operator*(const Matrix& m, const float v);
Matrix operator*(const float v, const Matrix& m);

struct Int2
{
	Int2() : x(0), y(0) {};
	Int2(int x, int y) :x(x), y(y) {};

	int x, y;

	Int2 operator-() const;
	Int2 operator+(const Int2& val) const;
	Int2 operator-(const Int2& val) const;
	Int2 operator+=(const Int2& val);
	Int2 operator-=(const Int2& val);
	bool operator==(const Int2& val) const;
};
