//==========================================================
// 物理的な動きを再現する計算を関数化したものをまとめる
#ifndef _INC_PHYSICS_H
#define _INC_PHYSICS_H

//==========================================================
// インクルード
#include	"MathTypeDefinition.h"

//==========================================================
// 関数
namespace NPhysics {
	// 内容
	// 　バネのシミュレーション
	// 戻り値
	// 　バネの力による加速度
	// 引数
	// 　_myPos：自分の座標
	// 　_targetPos：目標となる座標
	// 　_myMass：自分の質量
	// 　_naturalLen：バネの自然長
	SVector3D CalcSpringAcceleration(
		SVector3D _myPos,
		SVector3D _targetPos, 
		float _myMass, 
		float _naturalLen,
		float _stiffness);

}

#endif