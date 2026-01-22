//==========================================================
// ステージオブジェクトを識別する定数を定義する
#ifndef _INC_STAGE_OBJECT_ID_H
#define _INC_STAGE_OBJECT_ID_H



namespace NStageObjectID {
	enum EStageObjctID {
		TEST = 0,	// テスト用（サンプル）

		//=================================================
		// ステージオブジェクトの追加がしやすいように
		// 各種類１００刻みでIDを設定している
		//=================================================
		// ステージの基本パーツ（ID:１００～）-------------
		NORMAL_H = 100,		// 2*1	o
		LONG_H = 101,		// 3*1	o
		SHORT_BLOCK = 102,	// 1*1	o
		SQUARE = 110,		// 2*2	o

		// ステージ基本パーツ縦バージョン（ID:２００～）---
		NORMAL_V = 200,		// 1*2	o
		LONG_V = 201,		// 1*3	o

		// かどパーツ（ID:５００～）-----------------------
		CORNER = 500,		// 角	o
	};
}

//==========================================================
// 全ステージオブジェクトのヘッダをインクルード
#include	"StageObjTest.h"

#include	"StageObjNormalH.h"
#include	"StageObjLong.h"
#include	"StageObjShort.h"
#include	"StageObjSquare.h"
#include	"StageObjNormalV.h"
#include	"StageObjLongV.h"
#include	"StageObjCorner.h"

#endif