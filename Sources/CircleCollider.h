//==========================================================
// 円を使った当たり判定
#ifndef _INC_CIRCLE_COLLIDER_H
#define _INC_CIRCLE_COLLIDER_H

//==========================================================
// インクルード
#include	"BaseCollider.h"

//==========================================================
// クラス定義
class CCircleCollider : public CBaseCollider {
public:
	// 半径を設定
	void SetRadius(float _radius){ radius = _radius; polyCollider.SetSize(radius * 2, radius * 2); }

	float GetRadius(){ return radius; }

private:
	// あたり判定の半径の大きさ。
	float radius;

public:
	CCircleCollider();
	~CCircleCollider();
};


#endif