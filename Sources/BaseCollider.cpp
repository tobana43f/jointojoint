//==========================================================
// インクルード
#include	"BaseCollider.h"
#include	"Allocator.h"

//==========================================================
// グローバル変数
CTexture *CBaseCollider::pTexRefCircle = NULL;

//==========================================================
// メソッド定義
CBaseCollider::CBaseCollider(){
	SetIsActive(true);
	pOwnerPolygon3D = NULL;
	relativePosition = SVector3D(0, 0, 0);

	polyCollider.Init();
}

CBaseCollider::~CBaseCollider(){
	pOwnerPolygon3D = NULL;
	polyCollider.SetReferenceTexture(NULL);
}

void CBaseCollider::Update(){
	// 仮想関数
	// 必要なら継承後に定義
}


void CBaseCollider::Sync(){
	SVector3D	worldPos_ = GetWorldPositionCenter();

	// あたり判定用ポリゴンの座標を同期させる
	polyCollider.SetPositionCenter(worldPos_.x, worldPos_.y, worldPos_.z);
}

void CBaseCollider::Render(){
	if (!GetIsActive())
		return;

	//polyCollider.Render();
}

SVector3D CBaseCollider::GetWorldPositionCenter(){
	if (pOwnerPolygon3D == NULL)
		return relativePosition;

	return(pOwnerPolygon3D->GetPositionCenter() + relativePosition);
}