//==========================================================
// インクルード
#include	"GameOverManager.h"
#include	"CollideDetecter.h"
#include <math.h>

//==========================================================
// 名前空間
using namespace NCollideDetecter;

//==========================================================
// 静的領域
CGameState *CGameOverManager::pGameState = NULL;
CAllCharacterManager *CGameOverManager::pCharacter = NULL;
CAllStageObjectManager *CGameOverManager::pStageObj = NULL;
CAllJointableObjectManager *CGameOverManager::pJointableObj = NULL;

//==========================================================
// メソッド定義
CGameOverManager::CGameOverManager(){

}

CGameOverManager::~CGameOverManager(){

}

void CGameOverManager::Init(){
	limitBottom = 0;
	polGameArea.Init();
	polGameArea.SetColor(255, 255, 255, 64);

	colGameArea.SetRefOwner(&polGameArea);
	
	SetGameArea();

	//polGameArea.SetSize(1200, 120);
	//polGameArea.SetPositionCenter(600, 0, 0);
}

void CGameOverManager::Destroy(){

}

void CGameOverManager::Update(){
	colGameArea.Update();
	polGameArea.Update();
	CollideArea();
}

void CGameOverManager::Render(){
	//polGameArea.Render();
}

void CGameOverManager::SetRefInfo(
	CAllCharacterManager *_pRefChar,
	CAllStageObjectManager* _pRefStage,
	CAllJointableObjectManager *_pRefJointable)
{
	pCharacter = _pRefChar;
	pStageObj = _pRefStage;
	pJointableObj = _pRefJointable;
}

void CGameOverManager::SetGameArea(){
	SVector3D areaPosLT_(0, 0, 0);
	SVector3D areaPosRB_(0, 0, 0);

	// ステージオブジェクトの位置を参照
	{
		CBaseStageObject **objArray_ = pStageObj->GetStageObjArray();
		int objNum_ = pStageObj->GetObjNum();

		// オブジェクトの数
		for (int objIdx_ = 0; objIdx_ < objNum_; ++objIdx_){
			CBlockObject *blcArray_ = objArray_[objIdx_]->GetBlockObjArray();
			int blockNum_ = objArray_[objIdx_]->GetBlockNum();

			// オブジェクト内のブロックの数
			for (int blcIdx_ = 0; blcIdx_ < blockNum_; ++blcIdx_){
				SVector3D posCenter_ = blcArray_[blcIdx_].GetPositionCenter();

				// 左上判断
				if (posCenter_.x < areaPosLT_.x)
					areaPosLT_.x = posCenter_.x;
				if (posCenter_.y > areaPosLT_.y)
					areaPosLT_.y = posCenter_.y;

				// 右端判断
				if (posCenter_.x > areaPosRB_.x)
					areaPosRB_.x = posCenter_.x;
				if (posCenter_.y < areaPosRB_.y)
					areaPosRB_.y = posCenter_.y;
			}
		}
	}

	// ステージオブジェクトの位置を参照
	{
		CBaseJointableObject **objArray_ = pJointableObj->GetObjArray();
		int objNum_ = pJointableObj->GetObjNum();

		// オブジェクトの数
		for (int objIdx_ = 0; objIdx_ < objNum_; ++objIdx_){
			CBlockObject *blcArray_ = objArray_[objIdx_]->GetBlockObjArray();
			int blockNum_ = objArray_[objIdx_]->GetBlockNum();

			// オブジェクト内のブロックの数
			for (int blcIdx_ = 0; blcIdx_ < blockNum_; ++blcIdx_){
				SVector3D posCenter_ = blcArray_[blcIdx_].GetPositionCenter();

				// 左上判断
				if (posCenter_.x < areaPosLT_.x)
					areaPosLT_.x = posCenter_.x;
				if (posCenter_.y > areaPosLT_.y)
					areaPosLT_.y = posCenter_.y;

				// 右端判断
				if (posCenter_.x > areaPosRB_.x)
					areaPosRB_.x = posCenter_.x;
				if (posCenter_.y < areaPosRB_.y)
					areaPosRB_.y = posCenter_.y;
			}
		}
	}

	// 適用
	SVector3D size_ = areaPosRB_ - areaPosLT_;
	size_.x = fabs(size_.x);
	size_.y = fabs(size_.y);
	SVector3D posC_;// = (size_ / 2.0f) + areaPosLT_;
	posC_.x = size_.x / 2.0f + areaPosLT_.x;
	posC_.y = size_.y / 2.0f + areaPosRB_.y;
	posC_.z = 0;
	polGameArea.SetPositionCenter(posC_.x, posC_.y, posC_.z);
	polGameArea.SetSize(size_.x, size_.y);

	limitBottom = areaPosRB_.y - 80;	// 適当に下に下げる。
}

void CGameOverManager::CollideArea(){
	CCharacter *pChar_ = pCharacter->GetPActiveCharManager()->GetPCharacterListHead();
	bool isGameover_ = true;

	while (pChar_ != NULL){
		//// 四角形の判定でとるバージョン
		//bool isCollide_ = false;
		//CAABBCollider *colChar = pChar_->body.GetPCollideBroad();
		//isCollide_ = CollideAABBAndAABB(colChar, &colGameArea);

		//if (isCollide_){
		//	isGameover_ = false;
		//	break;
		//}

		// 下端の線でとるバージョン
		float yChar_ = pChar_->body.GetPositionCenter().y;
		if (yChar_ > limitBottom){
			isGameover_ = false;
			break;
		}

		// 次へ
		pChar_ = pChar_->next;
	}

	if (isGameover_){
		// キャラクターが1体もエリアに触れていない場合はゲームオーバー
		pGameState->isGameOver = true;
	}
	else{
		pGameState->isGameOver = false;
	}
}