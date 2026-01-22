//==========================================================
// インクルード
#include	"BaseAreaObject.h"
#include	"CollideDetecter.h"

//==========================================================
// 名前空間
using namespace NCollideDetecter;

//==========================================================
// 静的領域

CAllCharacterManager *CBaseAreaObject::pRefCharacter = NULL;

CTexture	*CBaseAreaObject::pRefTexTutorialBack = NULL;

//==========================================================
// メソッド定義

CBaseAreaObject::CBaseAreaObject(){
	colArea.SetRefOwner(&polyArea);
	SetIsCollide(false);
	cnt = 0;
	cntStart = cnt;
}

CBaseAreaObject::~CBaseAreaObject(){

}

void CBaseAreaObject::Init(SVector3D _initPos){
	// あたり判定にオーナーを設定
	colArea.SetColor(255, 0, 0, 64);

	// ポリゴン初期化
	polyArea.Init();

	// 座標の初期化
	SVector3D	initPos_(_initPos);
	polyArea.SetPositionCenter(initPos_.x, initPos_.y, initPos_.z);

	cnt = 0;
	cntStart = cnt;

	// 継承後に細かく設定
}

void CBaseAreaObject::Destroy(){

}

void CBaseAreaObject::Update(){
	// あたり判定の更新(たぶん不要)
	colArea.Update();
	cnt++;
}

void CBaseAreaObject::Sync(){
	// あたり判定を同期
	colArea.Sync();
}

void CBaseAreaObject::Render(){
	// あたり判定確認用
	colArea.Render();
	polyArea.Render();
}

void CBaseAreaObject::CollideArea(){
	CAABBCollider colCurChar_ = *GetPCharacter()->GetPCurCharacter()->body.GetPCollideBroad();

	bool isCollide_ = false;
	isCollide_ = CollideAABBAndAABB(&colCurChar_, &colArea);

	SetIsCollide(isCollide_);
}
