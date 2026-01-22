//==========================================================
// インクルード

#include	"ActiveCharacterManager.h"
#include	"Allocator.h"
#include	"Math.h"

//==========================================================
// 名前空間
using namespace NControllerDefinition;
using namespace NMath;

//==========================================================
// メソッド定義

CActiveCharacterManager::CActiveCharacterManager(){
	// リストの先頭にダミーデータを確保
	pCharacterListHead = new CCharacter();
	pCharacterListHead->prev = NULL;
	pCharacterListHead->next = NULL;

	// 最後尾を設定
	pCharacterListTail = pCharacterListHead;
}

CActiveCharacterManager::~CActiveCharacterManager(){
	Destroy();
}

void CActiveCharacterManager::Init(){
	pCharacterListHead->prev = NULL;
	pCharacterListHead->next = NULL;

	// 最後尾を設定
	pCharacterListTail = pCharacterListHead;
}

void CActiveCharacterManager::Destroy(){
	// 先頭の要素だけは確保しているので、解放する
	DeleteSafely(pCharacterListHead);
}

void CActiveCharacterManager::Update(){
	OperateCharacter();

	ProcDecVelocity();

	ApllyAngularAcceleration();

	UpdateCharacterList();

	ProcSpringMovement();

	ProcAdjustCore();

	ApllyAcceleration();

}

void CActiveCharacterManager::Sync(){
	SyncCharacterListPosition();
}

void CActiveCharacterManager::Render(){
	RenderCharacterList();
}

void CActiveCharacterManager::SetRefCharacterListTop(CCharacter *_pCharacter){
	pCharacterListHead->next = _pCharacter;			// ダミーデータを除く先頭に設定
	_pCharacter->prev = pCharacterListHead;			// 後ろの参照を設定
	_pCharacter->next = NULL;						// リストの最後尾として設定
	pCharacterListTail = pCharacterListHead->next;	// 最後尾を設定
}

//----------------------------------------------------------
// ユーティリティ

void CActiveCharacterManager::ProcDecVelocity(){
	CCharacter *target_ = pCharacterListTail;
	target_->velocityCore.x *= 0.8f;
	target_->velocityCore.y *= 0.8f;
}

void CActiveCharacterManager::UpdateCharacterList(){
	CCharacter	*pCur_ = pCharacterListHead->next;

	while (pCur_ != NULL){
		pCur_->Update();
		pCur_->polyCore.SetReferenceTexture(CCharacter::pTexRefCoreActive);
		pCur_ = pCur_->next;
	}
}

void CActiveCharacterManager::SyncCharacterListPosition(){
	CCharacter	*cur = pCharacterListTail->prev;	// 最後尾の1つ後ろ

	if (cur == NULL)								// キャラクターが1体もいない場合
		return;

	while (cur != pCharacterListHead){				// ダミーデータをまで
		cur->polyCore.SetPositionCenter(
			cur->next->body.GetPositionCenter().x,
			cur->next->body.GetPositionCenter().y,
			cur->next->body.GetPositionCenter().z
			);
		cur = cur->prev;
	}
}

void CActiveCharacterManager::RenderCharacterList(){
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

void CActiveCharacterManager::OperateCharacter(){
	//----------------------------------------------------------
	// 操作する対象を選択
	// リスト最後尾のコア
	CCharacter *target_ = pCharacterListTail;

	// もし、先頭のコアがほかのオブジェクトにつながっていた場合は操作しない
	if (target_->GetIsConnected())
		return;

	//----------------------------------------------------------
	// 加速量
	SVector3D	incBodySpeed_(0.32f, 0.165f, 0);	// ボディの動く速さ
	SVector3D	incCoreSpeed_(0.8f,0.8f, 0);
	SVector3D	newBodyAcc_(0, 0, 0);
	SVector3D	newCoreAcc_(0, 0, 0);

	//----------------------------------------------------------
	// 加速処理
	// 水平方向
	//if (pRefInput1->GetHorizontalPositive(HOLD))
	if(pRefInput1->GetHoldStickRight())
	{
		newBodyAcc_.x += incBodySpeed_.x;
		newCoreAcc_.x += incCoreSpeed_.x;
	}
	//else if (pRefInput1->GetHorizontalNegative(HOLD))
	else if(pRefInput1->GetHoldStickLeft())
	{
		newBodyAcc_.x -= incBodySpeed_.x;
		newCoreAcc_.x -= incCoreSpeed_.x;

	}

	// 垂直方向
	//if (pRefInput1->GetVerticalPositive(HOLD))
	if(pRefInput1->GetHoldStickUp())
	{
		//newBodyAcc_.y += incBodySpeed_.y;
		newCoreAcc_.y += incCoreSpeed_.y;

	}
	//else if (pRefInput1->GetVerticalNegative(HOLD))
	else if(pRefInput1->GetHoldStickDown())
	{
		//newBodyAcc_.y -= incBodySpeed_.y;
		newCoreAcc_.y -= incCoreSpeed_.y;
	}

	// 垂直方向
	//if (pRefInput1->GetVerticalPositive(TRIGGER) && target_->GetIsJumpReady())
	if(pRefInput1->GetTriggerStickUp() && target_->GetIsJumpReady())
	{
		newBodyAcc_.y += incBodySpeed_.y * 25;
		newCoreAcc_.y -= incCoreSpeed_.y * 2;
	}

	target_->body.AddAcceleration(newBodyAcc_);
	// 実際の移動量で回転するようにする
	target_->body.AddAngularAccelerationDegree(newBodyAcc_.x * -6);
	target_->velocityCore += newCoreAcc_;

	//target_->polyCore.SetPositionCenter(
	//	target_->body.GetPositionCenter().x,
	//	target_->body.GetPositionCenter().y,
	//	target_->body.GetPositionCenter().z
	//	);


	////----------------------------------------------------------
	//// 実際に反映
	target_->polyCore.AddPositionCenter(
		target_->velocityCore.x,
		target_->velocityCore.y ,
		0
		);
}

void CActiveCharacterManager::ProcSpringMovement(){
	CCharacter	*pCur_ = pCharacterListHead->next;
	float		naturalLen_ = CCharacter::GetCableNaturalLength();
	float		topNaturalLen = CCharacter::GetCableNaturalLength();

	corePullAcc = SVector3D(0, 0, 0);

	//----------------------------------------------------------
	// 全キャラクターにバネ
	while (pCur_ != NULL){
		SVector3D	sprAcc(0, 0, 0);

		SVector3D	body_ = pCur_->body.GetPositionCenter();
		float		mBody_ = CCharacter::GetBodyMass();

		
		//----------------------------------------------------------
		// 例外
		// 1っ体だけの時
		if (pCharacterListHead->next == pCharacterListTail){
			SVector3D	core_ = pCur_->polyCore.GetPositionCenter();
			float		mCore_ = CCharacter::GetCoreMass();

			// ボディへの影響
			//sprAcc = sprAcc + CalcSpringAcceleration(body_, core_, mBody_, topNaturalLen);
			//pCur_->body.AddAcceleration(sprAcc);
			//pCur_->body.AddAngularAccelerationDegree(-sprAcc.x * 2);
			//

			// コアへの影響
			SVector3D corePullAcc_ = CalcSpringAcceleration(core_, body_, mCore_, topNaturalLen);
			//pCur_->velocityCore = pCur_->velocityCore + corePullAcc_;
			pCur_->acceleartionCore += corePullAcc_;

			break;	// 終わり
		}

		//----------------------------------------------------------
		// キャラすべて

		// 先頭のキャラ
		if (pCur_ == pCharacterListTail){
			
			SVector3D	bodyBack_ = pCur_->prev->body.GetPositionCenter();
			SVector3D	core_ = pCur_->polyCore.GetPositionCenter();
			float		mCore_ = CCharacter::GetCoreMass();

			//// ボディへの影響
			//sprAcc = sprAcc + CalcSpringAcceleration(body_, core_, mBody_, topNaturalLen);
			//sprAcc = sprAcc + CalcSpringAcceleration(body_, bodyBack_, mBody_, naturalLen_);

			// コアへの影響
			SVector3D corePullAcc_ = CalcSpringAcceleration(core_, body_, mCore_, topNaturalLen);
			//pCur_->velocityCore = pCur_->velocityCore + corePullAcc_;
			pCur_->acceleartionCore += corePullAcc_;
			
		}
		// 最後尾のキャラ
		else if (pCur_ == pCharacterListHead->next){
			SVector3D	bodyFront_ = pCur_->next->body.GetPositionCenter();
			SVector3D	corePullAcc_ = CalcSpringAcceleration(body_, bodyFront_, mBody_, naturalLen_);

			sprAcc = sprAcc + corePullAcc_;
		}
		// それ以外(中間)
		else{
			SVector3D	bodyBack_ = pCur_->prev->body.GetPositionCenter();
			SVector3D	bodyFront_ = pCur_->next->body.GetPositionCenter();
			SVector3D	corePullAcc_ = CalcSpringAcceleration(body_, bodyBack_, mBody_, naturalLen_);

			sprAcc = sprAcc + corePullAcc_;
			sprAcc = sprAcc + CalcSpringAcceleration(body_, bodyFront_, mBody_, naturalLen_);
		}

		// 両方の影響を合成させたものを適用する。
		pCur_->body.AddAcceleration(sprAcc);
		// 実際の移動量で回転するようにする
		//pCur_->body.AddAngularAccelerationDegree(sprAcc.x * -0.5);

		// 次へ
		pCur_ = pCur_->next;
	}
}

void CActiveCharacterManager::ApllyAcceleration(){
	CCharacter	*pCur_ = GetPCharacterListHead();

	while (pCur_ != NULL){
		pCur_->velocityCore += pCur_->acceleartionCore;
		pCur_->acceleartionCore = SVector3D(0,0,0);
		pCur_ = pCur_->next;
	}
}

void CActiveCharacterManager::ApllyAngularAcceleration(){
	CCharacter	*pCur_ = GetPCharacterListHead();

	while (pCur_ != NULL){
		SVector3D prevPos_ = pCur_->body.GetPrevData().positionCenter;
		SVector3D curPos_ = pCur_->body.GetPositionCenter();
		SVector3D moveDist_ = curPos_ - prevPos_;

 		pCur_->body.AddAngularAccelerationDegree(-moveDist_.x);

		pCur_ = pCur_->next;
	}
}


void CActiveCharacterManager::AddCharacterToListTail(CCharacter *_pCharacter){
	// 最後尾に追加
	pCharacterListTail->next = _pCharacter;

	// 後ろの参照を設定
	_pCharacter->prev = pCharacterListTail;
	
	// 最後尾として設定
	_pCharacter->next = NULL;

	// 最後尾の変数を更新
	pCharacterListTail = pCharacterListTail->next;

}

CCharacter* CActiveCharacterManager::SubCharacterFromListTail(){
	// キャラクターが1体しかいないときの判定
	// ダミーデータを除く先頭が最後尾と等しい
	if (pCharacterListHead->next == pCharacterListTail)
		return NULL;

	// キャラクターが0体の時(テスト時の名残)
	if (pCharacterListTail->prev == NULL)
		return NULL;

	// 外したキャラクターのポインタを取得
	CCharacter *subChar_ = pCharacterListTail;

	// 最後尾を更新
	pCharacterListTail = pCharacterListTail->prev;
	pCharacterListTail->next = NULL;

	return subChar_;
}

SVector3D CActiveCharacterManager::CalcSpringAcceleration(SVector3D _myPos, SVector3D _tagPos, float _myMass, float _naturalLen){
	SVector3D	myPos_ = _myPos;
	SVector3D	tagPos_ = _tagPos;
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

		SVector3D sprF = len * CCharacter::GetSpringStiffness();
		SVector3D sprA = sprF / _myMass;

		// 求めた力を加算
		sprAcc_ = sprA;
	}
	return sprAcc_;
}

void CActiveCharacterManager::ProcAdjustCore(){
	CCharacter	*pCur_ = pCharacterListTail;
	float		topNaturalLen = 0;

	corePullAcc = SVector3D(0, 0, 0);

	if (fabs(pCur_->body.GetVelocity().x) > 0.1f)
		return;

	//----------------------------------------------------------
	// 全キャラクターにバネ
	SVector3D	sprAcc(0, 0, 0);

	SVector3D	body_ = pCur_->body.GetPositionCenter();
	float		mBody_ = CCharacter::GetBodyMass();


	//----------------------------------------------------------
	// 例外
	// 1っ体だけの時
	SVector3D	core_ = pCur_->polyCore.GetPositionCenter();
	float		mCore_ = CCharacter::GetCoreMass();

	// コアへの影響
	SVector3D corePullAcc_ = CalcSpringAcceleration(core_, body_, 50, topNaturalLen);
	pCur_->acceleartionCore += corePullAcc_;
}