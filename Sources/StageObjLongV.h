//==========================================================
// ステージオブジェクトの長いやつ
#ifndef _INC_STAGE_OBJECT_LONG_V_H
#define _INC_STAGE_OBJECT_LONG_V_H

//==========================================================
// インクルード
#include	"BaseStageObj.h"	// 継承元としてインクルード

//==========================================================
// クラス定義
class CStageObjLongV : public CBaseStageObject  {
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
	CStageObjLongV();
	~CStageObjLongV();
};

#endif