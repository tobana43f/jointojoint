//==========================================================
// 衝突検地を行う関数群の定義
#ifndef _INC_COLLIDE_DETECTER_H
#define _INC_COLLIDE_DETECTER_H

//==========================================================
// インクルード
#include	"BaseCollider.h"
#include	"CircleCollider.h"
#include	"AABBCollider.h"

//==========================================================
// 定義
#undef NULL
#define NULL (0)

//==========================================================
// 名前空間
namespace NCollideDetecter {

	// 円と円の当たり判定
	// メモ)
	// 　_pOutCollideLen ： 衝突し、めり込んでいる長さを取得する先のポインタ。
	// 注意)
	// 　2Dでのあたり判定なのでZ軸は無視される
	bool CollideCircleAndCircle(CCircleCollider *_pObj1, CCircleCollider *_pObj2, float *_pOutCollideLen = NULL);

	// AABBとAABBのあたり判定
	// メモ)
	// 　_pOutPosCenter ： 衝突しているボックスの中心座標を求める
	// 　_pOutSize　　　： 衝突しているボックスのサイズを求める
	bool CollideAABBAndAABB(CAABBCollider *_pObj1, CAABBCollider *_pObj2, SVector3D *_pOutPosCenter = NULL, SVector3D *_pOutSize = NULL);

	// 三角形と、点のあたり判定
	bool CollideTriangleAndPoint(SVector3D *_pPosA, SVector3D *_pPosB, SVector3D *_pPosC, SVector3D *_pPosP);


}

#endif