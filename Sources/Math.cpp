#include	"PlatformSelector.h"
#include	"Math.h"
#include	<math.h>

#ifdef _PLATFORM_DIRECTX
#include	<d3dx9.h>
#endif

//==========================================================
// 定数
float NMath::GetPI(){ return 3.1415926535897932384626433832795f; }

//==========================================================
// 角度変換
float NMath::ConvertDegToRad(float _deg){
#ifdef _PLATFORM_DIRECTX
	return D3DXToRadian(_deg);
#endif
}

float NMath::ConvertRadToDeg(float _rad){
#ifdef _PLATFORM_DIRECTX
	return D3DXToDegree(_rad);
#endif
}

//==========================================================
// プラットフォーム専用の型に変換
void NMath::ConvertShareTypeMatrixToSMatrix44(SMatrix44 *_dest, ShareTypeMatrixArg _src) {
	SMatrix44			converted_;
	
#ifdef _PLATFORM_DIRECTX
	// 行列のコピー
	for (int row = 0; row < 4; ++row){
		for (int col = 0; col < 4; ++col){
			_dest->matrix.element[row][col] = _src->m[row][col];
		}
	}

#endif
}

// 注意）
// 　DirectXではこの関数は使用しない
void NMath::ConvertShareTypeMatrix44ToSMatrix44(SMatrix44 *_dest, ShareTypeMatrix44Arg _src) {
#ifdef _PLATFORM_DIRECTX
	// なし
#endif
}

void NMath::ConvertSMatrix44ToShareTypeMatrix(ShareTypeMatrixArg _dest, SMatrix44 _src){
	SMatrix44			srcData_ = _src;

#ifdef _PLATFORM_DIRECTX
	// 行列のコピー
	for (int row = 0; row < 4; ++row){
		for (int col = 0; col < 4; ++col){
			_dest->m[row][col] = srcData_.matrix.element[row][col];
		}
	}
#endif
}

// 注意）
// 　DirectXではこの関数は使用しない。
void NMath::ConvertSMatrix44ToShareTypeMatrix44(ShareTypeMatrix44Arg _dest, SMatrix44 _src){
	SMatrix44	srcData_ = _src;

#ifdef _PLATFORM_DIRECTX
	// なし
#endif
}

//==========================================================
// ベクトルの計算
//----------------------------------------------------------
// 3D

// 内積を求める
float NMath::CalcDotVector3D(SVector3D *_pVector1, SVector3D *_pVector2){
	float dot_(0);

	dot_ = (_pVector1->x * _pVector2->x) + (_pVector1->y * _pVector2->y) + (_pVector1->z * _pVector2->z);

	return dot_;
}

// 外積を取得する
SVector3D NMath::CalcCrossVector3D(SVector3D *_pVector1, SVector3D *_pVector2){
	SVector3D	cross_(0, 0, 0);

	cross_.x = _pVector1->y * _pVector2->z - _pVector1->z * _pVector2->y;
	cross_.y = _pVector1->z * _pVector2->x - _pVector1->x * _pVector2->z;
	cross_.z = _pVector1->x * _pVector2->y - _pVector1->y * _pVector2->x;

	return cross_;
}

// ベクトルの長さを取得する
float NMath::CalcVector3DLength(SVector3D *_pVector){
	float len_(0);
	float powX_, powY_, powZ_;

	powX_ = _pVector->x * _pVector->x;
	powY_ = _pVector->y * _pVector->y;
	powZ_ = _pVector->z * _pVector->z;

	len_ = sqrt(powX_ + powY_ + powZ_);
	return len_;
}

SVector3D	NMath::CalcVector3DNormal(SVector3D *_pVector){
	float len_ = CalcVector3DLength(_pVector);
	SVector3D	normal_(*_pVector);

	// ゼロ除算を防止
	if (len_ == 0.0f)
		return normal_;

	len_ = 1.0f / len_;	// 除算を乗算で行うため
	normal_.x *= len_;
	normal_.y *= len_;
	normal_.z *= len_;

	//normal_.x /= len_;
	//normal_.y /= len_;
	//normal_.z /= len_;
	
	return normal_;
}

float NMath::CalcShortestDistXY(SVector3D *_pLineStart, SVector3D *_pLineEnd, SVector3D *_pPoint, SVector3D *_pOutCrossPos){
	SVector3D	lineLen_ = *_pLineEnd - *_pLineStart;
	float		a = (lineLen_.x * lineLen_.x) + (lineLen_.y * lineLen_.y);

	SVector3D	lenStartToPoint_	= (*_pLineStart)- (*_pPoint);
	//SVector3D	lenEndToPoint_		= (*_pLineEnd) - (*_pPoint);


	// 線が点の場合
	float errorRange_ = 0.000f;
	if (fabs(a) <= fabs(0 + errorRange_)){
		SVector3D shortestPos_;
		shortestPos_ = (lineLen_ * 0.0f) + (*_pLineStart);

		if (_pOutCrossPos != NULL)
			*_pOutCrossPos = shortestPos_;

		return sqrt((lenStartToPoint_.x * lenStartToPoint_.x) + (lenStartToPoint_.y * lenStartToPoint_.y));
	}

	float	b = (lineLen_.x * lenStartToPoint_.x) + (lineLen_.y * lenStartToPoint_.y);
	float	t = -(b / a);

	// tの範囲
	if (t < 0.0f)
		t = 0.0f; 
	else if (t > 1.0f)
		t = 1.0f;

	// 線分上の最短距離となる点を求める
	SVector3D shortestPos_;
	shortestPos_ = (lineLen_ * t) + (*_pLineStart);

	if (_pOutCrossPos != NULL)
		*_pOutCrossPos = shortestPos_;

	// 斜辺を求める
	return sqrt((shortestPos_.x - _pPoint->x) * (shortestPos_.x - _pPoint->x) + (shortestPos_.y - _pPoint->y) * (shortestPos_.y - _pPoint->y));
}

//----------------------------------------------------------
// 2D
// 外積を取得する
float NMath::CalcCrossVector2D(SVector2D *_pVector1, SVector2D *_pVector2){
	float cross_;
	cross_ = (_pVector1->x * _pVector2->y) - (_pVector1->y * _pVector2->x);

	return cross_;
}

//==========================================================
// 行列の計算
// 単位行列の作成
SMatrix44	NMath::CalcMatrixIdentity(){
	SMatrix44	calc_;

	for (int i = 0; i < 4; ++i){
		for (int j = 0; j < 4; ++j){
			if (i == j)
				calc_.matrix.element[i][j] = 1;
			else
				calc_.matrix.element[i][j] = 0;
		}
	}

	return calc_;
}

SMatrix44	NMath::CalcMatrixInverse(SMatrix44 _src){
	SMatrix44	mx44_;

#ifdef _PLATFORM_DIRECTX
	D3DXMATRIX	mx_, src_;
	ConvertSMatrix44ToShareTypeMatrix(&mx_, _src);
	D3DXMatrixInverse(&mx_, NULL, &src_);
	ConvertShareTypeMatrixToSMatrix44(&mx44_, &mx_);
#endif
	return mx44_;
}

SMatrix44	NMath::CalcMatrixScale(float _x, float _y, float _z){
	SMatrix44	mx44_;

#ifdef _PLATFORM_DIRECTX
	D3DXMATRIX	mx_;
	D3DXMatrixScaling(&mx_, _x, _y, _z);
	ConvertShareTypeMatrixToSMatrix44(&mx44_, &mx_);
#endif

	return mx44_;
}

SMatrix44	NMath::CalcMatrixRotateX(float angleRad_){
	SMatrix44	mx44_;

#ifdef _PLATFORM_DIRECTX
	D3DXMATRIX	mx_;
	D3DXMatrixRotationX(&mx_, angleRad_);
	ConvertShareTypeMatrixToSMatrix44(&mx44_, &mx_);
#endif

	return mx44_;
}

SMatrix44	NMath::CalcMatrixRotateY(float angleRad_){
	SMatrix44	mx44_;

#ifdef _PLATFORM_DIRECTX
	D3DXMATRIX	mx_;
	D3DXMatrixRotationY(&mx_, angleRad_);
	ConvertShareTypeMatrixToSMatrix44(&mx44_, &mx_);
#endif

	return mx44_;
}

SMatrix44	NMath::CalcMatrixRotateZ(float angleRad_){
	SMatrix44	mx44_;

#ifdef _PLATFORM_DIRECTX
	D3DXMATRIX	mx_;
	D3DXMatrixRotationZ(&mx_, angleRad_);
	ConvertShareTypeMatrixToSMatrix44(&mx44_, &mx_);
#endif

	return mx44_;
}

SMatrix44	NMath::CalcMatrixRotate(float _radX, float _radY, float _radZ){
	SMatrix44	mRot_, mX_, mY_, mZ_;
	mX_ = CalcMatrixRotateX(_radX);
	mY_ = CalcMatrixRotateY(_radY);
	mZ_ = CalcMatrixRotateZ(_radZ);
	mRot_ = mX_ * mY_ * mZ_;
	return mRot_;
}

SMatrix44	NMath::CalcMatrixTranslate(float _x, float _y, float _z){
	SMatrix44	mx44_;

#ifdef _PLATFORM_DIRECTX
	D3DXMATRIX	mx_;
	D3DXMatrixTranslation(&mx_, _x, _y, _z);
	ConvertShareTypeMatrixToSMatrix44(&mx44_, &mx_);
#endif

	return mx44_;
}

//==========================================================
// 行列とベクトルの計算

// 4列4行の行列と3次元ベクトルの計算
// メモ)
// 　3次元ベクトルを4次元ベクトルとして扱う。
// 　戻るベクトルは3次元
SVector3D	NMath::CalcVector3DTransform(SMatrix44 *_pMx, SVector3D *_pVec3){
	SVector3D	calc_(0, 0, 0);
	int i = 0;
	calc_.x = (_pMx->matrix.element[i][0] * _pVec3->x) + (_pMx->matrix.element[i][1] * _pVec3->y) + (_pMx->matrix.element[i][2] * _pVec3->z) + (_pMx->matrix.element[i][3]);
	++i;
	calc_.y = (_pMx->matrix.element[i][0] * _pVec3->x) + (_pMx->matrix.element[i][1] * _pVec3->y) + (_pMx->matrix.element[i][2] * _pVec3->z) + (_pMx->matrix.element[i][3]);
	++i;
	calc_.z = (_pMx->matrix.element[i][0] * _pVec3->x) + (_pMx->matrix.element[i][1] * _pVec3->y) + (_pMx->matrix.element[i][2] * _pVec3->z) + (_pMx->matrix.element[i][3]);

	return calc_;
}