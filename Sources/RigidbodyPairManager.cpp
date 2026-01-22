//==========================================================
// インクルード
#include	"RigidbodyPairManager.h"
#include	"Allocator.h"

//==========================================================
// メソッド定義
CRigidbodyPairManager::CRigidbodyPairManager(){
	pairListTop = NULL;
	Init();
}

CRigidbodyPairManager::~CRigidbodyPairManager(){
	FreeAllList();
}

void CRigidbodyPairManager::Init(){
	// 全て開放し、初期状態を作る
	FreeAllList();

	// 先頭にダミーデータを確保
	pairListTop = new CRigidbodyPair;
	
	// 最後の要素として設定
	pairListTop->next = NULL;
}

void CRigidbodyPairManager::AddPair(CRigidBody *_pRigid1, CRigidBody *_pRigid2){
	// リストの先頭を取得
	CRigidbodyPair	*cur = pairListTop;

	// 最後の要素までリストを進める
	while (cur->next != NULL){
		cur = cur->next;
	}

	cur->next = new CRigidbodyPair;
	cur = cur->next;
	cur->SetPair(_pRigid1, _pRigid2);
	cur->next = NULL;
}

void CRigidbodyPairManager::CollideDetect(){
	CRigidbodyPair *cur = pairListTop->next;

	while (cur != NULL){
		cur->CollideDetect();
		cur = cur->next;
	}
}


void CRigidbodyPairManager::CollideResponse(){
	CRigidbodyPair *cur = pairListTop->next;

	while (cur != NULL){
		cur->CollideResponse();
		cur = cur->next;
	}
}

void CRigidbodyPairManager::FreeAllList(){
	CRigidbodyPair *cur = pairListTop;
	CRigidbodyPair *next = NULL;

	while (cur != NULL){
		next = cur->next;
		DeleteSafely(cur);
		cur = next;
	}
}

