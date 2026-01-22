//==========================================================
// ステージオブジェクトのテスト
#ifndef _INC_STAGE_OBJECT_TEST_H
#define _INC_STAGE_OBJECT_TEST_H

//==========================================================
// インクルード
#include	"BaseStageObj.h"

//==========================================================
// クラス定義
class CStageObjTest : public CBaseStageObject  {
public:
	//==========================================================
	// 関数

	void Init(SVector3D _initPos, float _initAngle);
	void Update();

private:
	//==========================================================
	// 変数


	//==========================================================
	// 関数

	void InitBlockObj();

public:
	CStageObjTest();
	~CStageObjTest();
};

#endif