//==========================================================
// テスト用
#ifndef _INC_JOINTABLE_OBJ_TEST_H
#define _INC_JOINTABLE_OBJ_TEST_H

//==========================================================
// インクルード
#include	"BaseJointableObj.h"

//==========================================================
// クラス定義
class CJointableObjPendulam: public CBaseJointableObject  {
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

	void UpdateCharacter();

	void InitBlockObj();

public:
	CJointableObjPendulam();
	~CJointableObjPendulam();
};

#endif