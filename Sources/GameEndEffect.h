//==========================================================
// ゲームオーバー時の演出や、ゲームオーバー中の操作を行う関数
// ゲームクリアの時も同様に行う
#ifndef _INC_GAME_END_EFFECT_H
#define _INC_GAME_END_EFFECT_H

//==========================================================
// インクルード
#include	"Polygon2D.h"
#include	"GameState.h"
#include	"Input.h"
#include	"Sound.h"

//==========================================================
// クラス定義
class CGameEndEffect {
public:
	//==========================================================
	// 定数
	
	enum EState{	// ゲームの状態をクラス内で使用する形にして保持する
		GAME,
		CLEAR,
		OVER,
	};
	
	enum ECommand {	// ゲーム終了後の操作によるコマンド
		NOTHING,	// 選択していない状態

		RESTART,
		BACK_TO_STAGE_SELECT,
		BACK_TO_TITLE,

		NEXT_STAGE,
	};

	//==========================================================
	// 関数
	void Load();
	void Init();
	void Update();
	void Render();

	bool GetIsFadeEnd(){ return isFadeEnd; }

	static void SetRefInput(CInput *_pRef){ pRefInput1 = _pRef; }
	static void SetRefInfo(CGameState *_pRef){ pGameState = _pRef; }

	ECommand	GetCommand();

private:
	//==========================================================
	// 変数

	// 参照する情報
	static CGameState	*pGameState;
	static CInput		*pRefInput1;

	// リソース
	CTexture	*texStrGameOver;
	CTexture	*texRestart;
	CTexture	*texBackToStageSelect;

	CTexture	*texStrStageClear;
	CTexture	*texStrPushA;

	CSound	sndSECursor;
	CSound	sndSEDecide;

	// 現在の状態
	EState state;
	bool isFadeEnd;
	bool isIntervalEnd;
	bool isEffectEnd;

	bool isGameOver;
	
	// フェードアウト用
	// メモ)	
	// 　描画は2D だけで行う
	CPolygon2D	polBlack;
	CPolygon2D	polWhite;

	// 選択肢など
	CPolygon2D	polHeadGameOver;
	CPolygon2D	polRestart;
	CPolygon2D	polBackToStageSelect;
	int cursorGameOver;

	CPolygon2D	polHeadStageClear;
	CPolygon2D	polPushA;



	// 汎用
	int cnt;
	int cntStart;

	ECommand	command;
	bool		isCommandSelected;

	//==========================================================
	// 関数
	
	void CheckGameState();

	void UpdateFade();
	void UpdateInterval();

	void UpdateGameClear();
	void RenderGameClear();

	void UpdateGameOver();
	void RenderGameOver();

	void UpdateGameOverCommand();
	void RenderGameOverCommand();

public:
	CGameEndEffect();
	~CGameEndEffect();
};

#endif