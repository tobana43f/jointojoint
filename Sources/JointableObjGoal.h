//==========================================================
// ゴール
#ifndef _INC_JOINTABLE_OBJ_GOAL_H
#define _INC_JOINTABLE_OBJ_GOAL_H

//==========================================================
// インクルード
#include	"BaseJointableObj.h"

//==========================================================
// クラス定義
class CJointableObjGoal : public CBaseJointableObject  {
public:
	//==========================================================
	// 関数

	void Init(SVector3D _initPos, float _initAngle);
	void Update();

	static void SetRefTexture(CTexture *_pRef){ pRefTexGoal = _pRef; }

private:
	//==========================================================
	// 変数

	static CTexture	*pRefTexGoal;


	//==========================================================
	// 関数

	void UpdateCharacter();

	void InitBlockObj();

public:
	CJointableObjGoal();
	~CJointableObjGoal();
};

#endif