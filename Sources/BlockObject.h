//==========================================================
// 足場や壁になる、基本のオブジェクトのクラス
//----------------------------------------------------------
// 剛体の四角
#ifndef _INC_BLOCK_OBJECT_H
#define _INC_BLOCK_OBJECT_H

//==========================================================
// インクルード
#include	"RigidBody.h"
#include	"AABBCollider.h"

//==========================================================
// クラス定義
class CBlockObject : public CRigidBody{
public:
	//==========================================================
	// 関数

	// 初期位置を指定しての初期化をする
	void Init();
	void Sync();
	void Render();

private:

public:
	CBlockObject(){};
	~CBlockObject(){};
};

#endif