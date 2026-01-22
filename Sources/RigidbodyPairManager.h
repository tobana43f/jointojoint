//==========================================================
// 衝突可能性がある剛体のペアのリストを管理するくらす
#ifndef _RIGID_BODY_PAIR_MANAGER_H
#define _RIGID_BODY_PAIR_MANAGER_H

//==========================================================
// インクルード
#include	"RigidbodyPair.h"

//==========================================================
// クラス定義
class CRigidbodyPairManager {
public:
	//==========================================================
	// 関数

	// 判定するペアは毎フレーム変わるので、毎フレーム判定前に必ず初期する。
	void Init();

	void AddPair(CRigidBody *_pRigid1, CRigidBody *_pRigid2);

	void CollideDetect();

	// 同じオブジェクトが2つ以上の判定応答をする場合は、必要な量だけに調整する
	//void CollideAdjust();

	void CollideResponse();

private:
	//==========================================================
	// 変数

	CRigidbodyPair	*pairListTop;

	//==========================================================
	// 関数

	// 全リスト解放
	// 先頭のダミーデータも削除する
	void FreeAllList();

public:
	CRigidbodyPairManager();
	~CRigidbodyPairManager();
};


#endif