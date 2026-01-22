//==========================================================
// インクルード
#include	"CharacterBody.h"

//==========================================================
// メソッド定義

CCharacterBody::CCharacterBody(){

}

CCharacterBody::~CCharacterBody(){

}

void CCharacterBody::Init(){
	CRigidBody::Init();

	// サイズの初期化
	SetSize(20, 20);

	SetType(DYNAMIC);
	SetMass(10.0f);
	SetFriction(0.52f);
	SetRestitution(0.0f);

	// あたり判定の色
	colBroad.SetRelativeSize(3, 3);
	colBroad.SetColor(0, 0, 255, 128);
}