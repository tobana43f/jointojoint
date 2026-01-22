//==========================================================
// ゲームの状況などのさまざまな情報を保持するクラス
#ifndef _INC_GAME_STATE_H
#define _INC_GAME_STATE_H

//==========================================================
// インクルード
#include	"StageID.h"

//==========================================================
// クラス定義
class CGameState {
public:
	//==========================================================
	// 定数

	// ゲームモード
	enum EGameMode{
		TEST,
		STORY,
		STAGE_SELECT,
	};

	int GetConstLifeMax(){ return 1; }

	//==========================================================
	// 関数

	void Init(EGameMode _mode, int _stageNo){
		gameMode = _mode;
		stageNo = _stageNo;
		lifeMax = GetConstLifeMax();
		lifeLeave = lifeMax;

		isGameOver = false;
		isGameClear = false;
	}

public:
	//==========================================================
	// 変数

	EGameMode	gameMode;

	int			stageNo;	// 現在のステージNo
	
	int			lifeMax;	// 残機の最大数
	int			lifeLeave;	// 残機

	bool		isGameOver;
	bool		isGameClear;

	//==========================================================
	// 関数



public:
	CGameState(){};
	~CGameState(){};

};

#endif