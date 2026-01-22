//==========================================================
// インクルード
#include	"NegativeCharacterManager.h"
#include	"Allocator.h"
#include	"Math.h"

//==========================================================
// 名前空間
using namespace NMath;

//==========================================================
// メソッド定義
CNegativeCharacterManager::CNegativeCharacterManager(){

	// リストの先頭にダミーデータを確保
	pCharacterListHead = new CCharacter();
	pCharacterListHead->prev = NULL;
	pCharacterListHead->next = NULL;

	// 最後尾を設定
	pCharacterListTail = pCharacterListHead;

}

CNegativeCharacterManager::~CNegativeCharacterManager(){
	Destroy();
}

void CNegativeCharacterManager::Init(){
	pCharacterListHead->prev = NULL;
	pCharacterListHead->next = NULL;

	// 最後尾を設定
	pCharacterListTail = pCharacterListHead;
}

void CNegativeCharacterManager::Destroy(){
	// 先頭の要素だけは確保しているので、解放する
	DeleteSafely(pCharacterListHead);
}

void CNegativeCharacterManager::Update(){
	CCharacter	*pCur_ = pCharacterListHead->next;

	ProcSpringMovement();

	// ApllyVelocity
	while (pCur_ != NULL){
		pCur_->Update();

		pCur_->velocityCore.x *= 0.8f;
		pCur_->velocityCore.y *= 0.8f;

		pCur_->polyCore.AddPositionCenter(
			pCur_->velocityCore.x,
			pCur_->velocityCore.y,
			0
			);

		pCur_->polyCore.SetReferenceTexture(CCharacter::pTexRefCoreNegative);

		pCur_ = pCur_->next;
	}
}

void CNegativeCharacterManager::Sync(){

}

void CNegativeCharacterManager::Render(){
	CCharacter	*pCur_ = pCharacterListHead->next;

	while (pCur_ != NULL){
		pCur_->polyCable.Render();
		pCur_ = pCur_->next;
	}

	pCur_ = pCharacterListHead->next;
	while (pCur_ != NULL){
		pCur_->polyCore.Render();
		pCur_->body.Render();

		pCur_->colGround.Render();
		pCur_->colliderCore.Render();
		pCur_->colliderCoreSpace.Render();
		

		pCur_ = pCur_->next;
	}
}

void CNegativeCharacterManager::AddCharacterToListTail(CCharacter *_pCharacter){
	if (_pCharacter == NULL)
		return;

	// 最後尾に追加
	pCharacterListTail->next = _pCharacter;

	// 後ろの参照を設定
	_pCharacter->prev = pCharacterListTail;

	// 最後尾として設定
	_pCharacter->next = NULL;

	// 最後尾の変数を更新
	pCharacterListTail = pCharacterListTail->next;

}

void CNegativeCharacterManager::SubTargetCharacterFromList(CCharacter *_pTarget){
	CCharacter *pCur = pCharacterListHead->next;	// 実態の先頭を取得

	// ターゲットを探索するために、全リストを走査
	while (pCur != NULL){
		// ターゲットを見つけた場合
		if (pCur == _pTarget)
			break;

		// 次へ
		pCur = pCur->next;
	}

	// 探索失敗(リスト内にターゲットが存在しない)
	if (pCur == NULL)
		return;

	// ターゲットの前後のリストをつなげる
	pCur->prev->next = pCur->next;

	if (pCur->next != NULL){		// 最後尾の場合はメモリ違反になる。
		pCur->next->prev = pCur->prev;
	}
	else{							// Tailの場合
		pCharacterListTail = pCur->prev;	// Tailを更新
	}

	// ターゲットをリストから外す
	pCur->prev = NULL;
	pCur->next = NULL;
}

void CNegativeCharacterManager::ProcSpringMovement(){
	CCharacter	*pCur_ = pCharacterListHead->next;


	// 全キャラクターにバネ
	while (pCur_ != NULL){
		SVector3D	sprAcc(0, 0, 0);
		SVector3D	body_ = pCur_->body.GetPositionCenter();
		float		mBody_ = CCharacter::GetBodyMass();
		SVector3D   core_ = pCur_->polyCore.GetPositionCenter();	
		float		mCore_ = CCharacter::GetCoreMass();

		pCur_->velocityCore = pCur_->velocityCore + CalcSpringAcceleration(core_, body_, mCore_);

		pCur_ = pCur_->next;
	}

}

SVector3D CNegativeCharacterManager::CalcSpringAcceleration(SVector3D _myPos, SVector3D _tagPos, float _myMass){
	SVector3D	myPos_ = _myPos;
	SVector3D	tagPos_ = _tagPos;
	SVector3D	objLen_ = (tagPos_ - myPos_);	// オブジェクト間の長さ
	SVector3D	sprAcc_(0, 0, 0);
	float		sprNatuLen_ = 1;

	// 自然超より長い場合のみ影響が出る
	if (CalcVector3DLength(&objLen_) > sprNatuLen_){
		float rad = atan2(objLen_.y, objLen_.x);	// ターゲットまでの角度を取得
		SVector3D natuVec(0, 0, 0);
		natuVec.x = sprNatuLen_ * cos(rad);
		natuVec.y = sprNatuLen_ * sin(rad);

		SVector3D len = objLen_ - natuVec;			// 自然長からの変化量(自然超以下は無視する)

		SVector3D sprF = len * 0.05f;
		SVector3D sprA = sprF / _myMass;

		// 求めた力を加算
		sprAcc_ = sprA;
	}
	return sprAcc_;
}