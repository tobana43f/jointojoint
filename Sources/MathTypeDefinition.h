//==========================================================
// プラットフォームに依存する型を、両方で使えるように再定義する
// メモ)
// 　数学関係の型が多いので、MathTypeの定義とする。
#ifndef _INC_MATH_TYPE_DEFINITION_H
#define _INC_MATH_TYPE_DEFINITION_H

//==========================================================
// 色データを保持する構造体
struct SColor {
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;	// 透明度
};

//==========================================================
// 座標に関わる構造体
// 三次元空間のベクトル
struct SVector3D {
	float x;
	float y;
	float z;

	SVector3D(){}
	SVector3D(float _x, float _y, float _z);

	SVector3D&	operator = (const SVector3D&);
	SVector3D	operator + (const SVector3D&);
	SVector3D	operator - (const SVector3D&);
	SVector3D	operator * (const SVector3D&);
	SVector3D	operator / (const SVector3D&);

	SVector3D	operator * (const float&);
	SVector3D	operator / (const float&);

	SVector3D&	operator += (const SVector3D&);
	
	bool		operator == (const SVector3D&);
	bool		operator != (const SVector3D&);
	
};

// 二次元ベクトル
struct SVector2D {
	float x;
	float y;

	SVector2D(){}
	SVector2D(float _x, float _y);

	SVector2D&	operator = (const SVector2D&);
	SVector2D	operator + (const SVector2D&);
	SVector2D	operator - (const SVector2D&);
	SVector2D	operator * (const float&);
	SVector2D	operator / (const float&);
};

// 3次元の整数型
struct SCoord3D {
	int x;
	int y;
	int z;
};

// 2次元の整数型
struct SCoord2D {
	int x;
	int y;
};

//==========================================================
// 始点と距離を持つ要素
struct SVecSegment2D{
	SVector2D	s;
	SVector2D	v;
};

struct SVecSegment3D{
	SVector3D	s;
	SVector3D	v;
};

//==========================================================
// 行列関係
// 4x4の行列　(DirectXの定義と同じ)
struct SMatrix44 {
	union {
		struct SElements	{		// 行列内の要素に直接アクセスできるようにする。
			float e11, e12, e13, e14;
			float e21, e22, e23, e24;
			float e31, e32, e33, e34;
			float e41, e42, e43, e44;
		} ;
		float element[4][4];		// 配列としてアクセスできる。
	}matrix;

	SMatrix44(){}
	//SMatrix44(SMatrix44& _src){ *this = _src; }	// operator= を邪魔してしまう？

	SMatrix44&	operator = (const SMatrix44&);
	SMatrix44	operator + (const SMatrix44&) const;
	SMatrix44	operator - (const SMatrix44&) const;
	SMatrix44	operator * (const SMatrix44&) const;
};

#endif