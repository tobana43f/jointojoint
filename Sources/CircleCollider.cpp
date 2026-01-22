//==========================================================
// インクルード
#include	"CircleCollider.h"

//==========================================================
// メソッド定義

CCircleCollider::CCircleCollider(){
	polyCollider.SetReferenceTexture(pTexRefCircle);
	radius = 0;
}

CCircleCollider::~CCircleCollider(){
	
}
