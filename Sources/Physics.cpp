//==========================================================
// インクルード
#include	"Physics.h"
#include	"Math.h"

//==========================================================
// 名前空間
using namespace NMath;

//==========================================================
// メソッド定義

SVector3D	NPhysics::CalcSpringAcceleration(
	SVector3D _myPos,
	SVector3D _targetPos, 
	float _myMass, 
	float _naturalLen,
	float _stiffness)
{
	SVector3D	myPos_ = _myPos;
	SVector3D	tagPos_ = _targetPos;
	SVector3D	objLen_ = (tagPos_ - myPos_);	// オブジェクト間の長さ
	SVector3D	sprAcc_(0, 0, 0);
	float		sprNatuLen_ = _naturalLen;

	// 自然超より長い場合のみ影響が出る
	if (CalcVector3DLength(&objLen_) > sprNatuLen_){
		float rad = atan2(objLen_.y, objLen_.x);	// ターゲットまでの角度を取得
		SVector3D natuVec(0, 0, 0);
		natuVec.x = sprNatuLen_ * cos(rad);
		natuVec.y = sprNatuLen_ * sin(rad);

		SVector3D len = objLen_ - natuVec;	// 自然長からの変化量(自然超以下は無視する)

		SVector3D sprF = len * _stiffness;
		SVector3D sprA = sprF / _myMass;

		// 求めた力を加算
		sprAcc_ = sprA;
	}
	return sprAcc_;
}