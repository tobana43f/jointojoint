//==========================================================
// 数学関係の処理をする関数群
// 行列は全て左手系(？)
#ifndef _INC_MATH_H
#define _INC_MATH_H

#include <Math.h>

// ・独自定義した構造体、クラスを
// 　両方のプラットフォームで用意されている関数を使用した演算を行えるようにする。
#include	"PlatformSelector.h"
#include	"MathTypeDefinition.h"	// 独自型の参照

// 変数の引数や戻り値に 専用を型を指定したい場合のために、
// 同じ名前で再定義する。
// 注意) Math.h Math.cpp 以外では使用しないこと。
#ifdef _PLATFORM_DIRECTX
#include	<d3dx9.h>

typedef D3DXMATRIX	ShareTypeMatrix;				// 命名規則 例外(共通の型名(ShareType))
struct  ShareTypeMatrix44 {};						

typedef ShareTypeMatrix		*ShareTypeMatrixArg;	
typedef ShareTypeMatrix44	*ShareTypeMatrix44Arg;

#endif

namespace NMath {
	//----------------------------------------------------------
	// 定数

	float GetPI();

	//----------------------------------------------------------
	// 変換

	// 角度の変換(度 ⇔ ラジアン)
	float ConvertDegToRad(float _deg);
	float ConvertRadToDeg(float _rad);

	// 独自型とプラットフォーム固有の型の変換
	void		ConvertShareTypeMatrixToSMatrix44(SMatrix44 *_dest, ShareTypeMatrixArg _src);
	void		ConvertShareTypeMatrix44ToSMatrix44(SMatrix44 *_dest, ShareTypeMatrix44Arg _src);

	void		ConvertSMatrix44ToShareTypeMatrix(ShareTypeMatrixArg _dest, SMatrix44 _src);
	void		ConvertSMatrix44ToShareTypeMatrix44(ShareTypeMatrix44Arg _dest, SMatrix44 _src);

	//----------------------------------------------------------
	// ベクトル演算

	// ベクトルの内積
	float		CalcDotVector3D(SVector3D *_pVector1, SVector3D *_pVector2);

	// ベクトル間の角度を求める(必要になったら実装する)
	//float		CalcCosTheta(SVector3D *_pVector1, SVector3D *_pVector2);

	// ベクトルの外積
	SVector3D	CalcCrossVector3D(SVector3D *_pVector1, SVector3D *_pVector2);

	// ベクトルの長さ
	float		CalcVector3DLength(SVector3D *_pVector);

	// ベクトルの正規化
	SVector3D	CalcVector3DNormal(SVector3D *_pVector);

	// XY平面の線分(ベクトル)と点の最短距離を求める
	// Zの値は無視する
	// メモ)
	// 　_pOutCrossPos : 線分上の最短距離となる座標
	float		CalcShortestDistXY(SVector3D *_pLineStart, SVector3D *_pLineEnd, SVector3D *_pPoint, SVector3D *_pOutCrossPos = NULL);

	// 2Dベクトルの外積
	float		CalcCrossVector2D(SVector2D *_pVector1, SVector2D *_pVector2);

	//----------------------------------------------------------
	// 行列演算

	// 行列計算
	SMatrix44	CalcMatrixIdentity();
	SMatrix44	CalcMatrixInverse(SMatrix44 _src);
	SMatrix44	CalcMatrixScale(float _x, float _y, float _z);
	SMatrix44	CalcMatrixRotateX(float angleRad_);
	SMatrix44	CalcMatrixRotateY(float angleRad_);
	SMatrix44	CalcMatrixRotateZ(float angleRad_);
	SMatrix44	CalcMatrixRotate(float _radX, float _radY, float _radZ);
	SMatrix44	CalcMatrixTranslate(float _x, float _y, float _z);

	//----------------------------------------------------------
	// 行列とベクトルの演算
	// 行列とベクトル
	SVector3D	CalcVector3DTransform(SMatrix44 *_pMx, SVector3D *_pVec3);
}

#endif