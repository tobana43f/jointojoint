//==========================================================
// ステージオブジェクトのテスト
#ifndef _INC_STAGE_OBJECT_NORMAL_H
#define _INC_STAGE_OBJECT_NORMAL_H

//==========================================================
// インクルード
#include	"BaseStageObj.h"	// 継承元としてインクルード

//==========================================================
// クラス定義
class CStageObjNormal : public CBaseStageObject  {
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
	CStageObjNormal();
	~CStageObjNormal();
};

#endif