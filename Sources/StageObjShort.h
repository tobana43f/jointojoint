//==========================================================
// ステージオブジェクトのテスト
#ifndef _INC_STAGE_OBJECT_SHORT_H
#define _INC_STAGE_OBJECT_SHORT_H

//==========================================================
// インクルード
#include	"BaseStageObj.h"	// 継承元としてインクルード

//==========================================================
// クラス定義
class CStageObjShort : public CBaseStageObject  {
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
	CStageObjShort();
	~CStageObjShort();
};

#endif