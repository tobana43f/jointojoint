//==========================================================
// エリアオブジェクトを識別する定数を定義する
#ifndef _INC_AREA_OBJECT_ID_H
#define _INC_AREA_OBJECT_ID_H

// 基本的にチュートリアルのオブジェクトが多い
namespace NAreaObjectID{
	enum EAreaObjectID{
		TEST = 0,

		//-----------------------------
		// チュートリアルオブジェクト
		// 移動系
		TUTORIAL_MOVE = 100,

		// ジャンプ
		TUTORIAL_JUMP = 110,

		// つなげる
		TUTORIAL_JOINT_CHAR = 120,	// キャラと
		TUTORIAL_JOINT_OBJ = 121,	// オブジェと
		TUTORIAL_JOINT_LIFT = 122,	// リフトと	// 今は使わない

		// 振り子
		TUTORIAL_PENDULAM = 130,	// 揺らす

		// 外す
		TUTORIAL_SEPARATE_CHAR = 140,	// キャラから
		TUTORIAL_SEPARATE_OBJ = 141,	// オブジェから

		// 落ちる
		TUTORILA_FALL = 150,	// 落ちちゃダメ
		///--------------------------------------------------
	};
}

//==========================================================
// 全ヘッダインクルード
#include	"AreaObjectTest.h"
#include	"AreaObjTutorialMove.h"
#include	"AreaObjTutorialJump.h"
#include	"AreaObjTutorialJointChar.h"
#include	"AreaObjTutorialJointObj.h"
#include	"AreaObjTutorialPendulam.h"
#include	"AreaObjTutorialSeparateChar.h"
#include	"AreaObjTutorialSeparateObj.h"
#include	"AreaObjTutorialFall.h"

#endif