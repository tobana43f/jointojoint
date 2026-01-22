//==========================================================
// 繋げられるオブジェクトを識別する定数を定義する
#ifndef _INC_JOINTABLE_OBJECT_ID_H
#define _INC_JOINTABLE_OBJECT_ID_H

namespace NJointableObjectID {
	enum EJointableObjctID {
		TEST = 0,

		GOAL = 900,
	};
}

//==========================================================
// 全ヘッダをまとめる
#include	"JointableObjPendulam.h"
#include	"JointableObjGoal.h"

#endif

