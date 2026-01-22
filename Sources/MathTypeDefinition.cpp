#include	"MathTypeDefinition.h"

//==========================================================
// ベクトル関係の演算子のオーバーライド

// SVector3D
SVector3D::SVector3D(float _x, float _y, float _z){
	x = _x; y = _y; z = _z;
}

SVector3D&	SVector3D::operator=(const SVector3D& _rhs){
	x = _rhs.x;
	y = _rhs.y;
	z = _rhs.z;
	return *this;
}

SVector3D SVector3D::operator+(const SVector3D& _rhs){
	return SVector3D(x + _rhs.x, y + _rhs.y, z + _rhs.z);
}

SVector3D SVector3D::operator-(const SVector3D& _rhs){
	return SVector3D(x - _rhs.x, y - _rhs.y, z - _rhs.z);
}

SVector3D SVector3D::operator*(const SVector3D& _rhs){
	return SVector3D(x * _rhs.x, y * _rhs.y, z * _rhs.z);
}

SVector3D SVector3D::operator/(const SVector3D& _rhs){
	return SVector3D(x / _rhs.x, y / _rhs.y, z / _rhs.z);
}


SVector3D SVector3D::operator*(const float& _rhs){
	return SVector3D(x * _rhs, y * _rhs, z * _rhs);
}

SVector3D SVector3D::operator/(const float& _rhs){
	return SVector3D(x / _rhs, y / _rhs, z / _rhs);
}

SVector3D&	SVector3D::operator+=(const SVector3D& _rhs){
	x += _rhs.x;
	y += _rhs.y;
	z += _rhs.z;
	return *this;
}

bool SVector3D::operator==(const SVector3D& _rhs){
	if (x == _rhs.x &&
		y == _rhs.y &&
		z == _rhs.z)
		return true;
	return false;
}

bool SVector3D::operator!=(const SVector3D& _rhs){
	if (x != _rhs.x &&
		y != _rhs.y &&
		z != _rhs.z)
		return true;
	return false;
}


// SVector2D
SVector2D::SVector2D(float _x, float _y){
	x = _x;
	y = _y;
}

SVector2D&	SVector2D::operator=(const SVector2D& _rhs){
	x = _rhs.x;
	y = _rhs.y;
	return *this;
}

SVector2D SVector2D::operator+(const SVector2D& _rhs){
	return SVector2D(x + _rhs.x, y + _rhs.y);
}

SVector2D SVector2D::operator-(const SVector2D& _rhs){
	return SVector2D(x - _rhs.x, y - _rhs.y);
}

SVector2D SVector2D::operator*(const float& _rhs){
	return SVector2D(x * _rhs, y * _rhs);
}

SVector2D SVector2D::operator/(const float& _rhs){
	return SVector2D(x / _rhs, y / _rhs);
}

// 行列計算
SMatrix44&	SMatrix44::operator=(const SMatrix44& _rhs){
	for (int i = 0; i < 4; ++i){
		for (int j = 0; j < 4; ++j){
			matrix.element[i][j] = _rhs.matrix.element[i][j];
		}
	}
	return *this;
}

SMatrix44	SMatrix44::operator+(const SMatrix44& _rhs) const {
	SMatrix44	calc_;
	for (int i = 0; i < 4; ++i){
		for (int j = 0; j < 4; ++j){
			calc_.matrix.element[i][j] = matrix.element[i][j] + _rhs.matrix.element[i][j];
		}
	}
	return calc_;
}

SMatrix44	SMatrix44::operator-(const SMatrix44& _rhs) const {
	SMatrix44	calc_;
	for (int i = 0; i < 4; ++i){
		for (int j = 0; j < 4; ++j){
			calc_.matrix.element[i][j] = matrix.element[i][j] - _rhs.matrix.element[i][j];
		}
	}
	return calc_;
}

SMatrix44	SMatrix44::operator*(const SMatrix44& _rhs) const{
	SMatrix44	calc_;

	for (int i = 0; i < 4; ++i){
		for (int j = 0; j < 4; ++j){

			float element_ = 0;
			for (int k = 0; k < 4; ++k){
				element_ += matrix.element[i][k] * _rhs.matrix.element[k][j];
			}

			calc_.matrix.element[i][j] = element_;
		}
	}

	return calc_;
}
