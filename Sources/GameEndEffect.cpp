//==========================================================
// インクルード
#include	"GameEndEffect.h"
#include	"System.h"
#include <stdlib.h>
#include	"AllTextureManager.h"

//==========================================================
// 名前空間
using namespace NSystem;
using namespace NControllerDefinition;
using namespace NAllTextureManager;

//==========================================================
// 静的領域
CGameState	*CGameEndEffect::pGameState = NULL;
CInput		*CGameEndEffect::pRefInput1 = NULL;

static SVector2D LETTER_SIZE(256, 32);

//==========================================================
// メソッド定義
CGameEndEffect::CGameEndEffect(){

}

CGameEndEffect::~CGameEndEffect(){

}

void CGameEndEffect::Load(){
	sndSECursor.Load(NSoundResourceInfo::SE_CURSOR);
	sndSEDecide.Load(NSoundResourceInfo::SE_DECIDE);

	texStrGameOver = GetPTexture(NTextureResourceInfo::STR_GAME_OVER);
	texStrStageClear = GetPTexture(NTextureResourceInfo::STR_STAGE_CLEAR);
	texRestart = GetPTexture(NTextureResourceInfo::RESTART_WHITE);
	texBackToStageSelect = GetPTexture(NTextureResourceInfo::STAGE_SELECT_WHITE);
	texStrPushA = GetPTexture(NTextureResourceInfo::PUSH_A_BUTTON);
}

void CGameEndEffect::Init(){
	cnt = 0;
	cntStart = cnt;
	
	command = NOTHING;

	state = GAME;
	isFadeEnd = false;
	isIntervalEnd = false;
	isEffectEnd = false;
	isGameOver = false;
	isCommandSelected = false;

	polBlack.Init();
	polBlack.SetSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	polBlack.SetPositionLeftTop(0, 0);
	polBlack.SetColor(0, 0, 0, 0);

	polWhite.Init();
	polWhite.SetSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	polWhite.SetPositionLeftTop(0, 0);
	polWhite.SetColor(255, 255, 255, 0);

	//----------------------------------------------------------
	// クリア
	float strWidRatio_ = 4;
	float strHeiRatio_ = 1;
	float headerSize_ = 150;
	polHeadStageClear.Init();
	polHeadStageClear.SetSize((strWidRatio_ + 2) * headerSize_, strHeiRatio_ * headerSize_);
	polHeadStageClear.SetPositionCenter(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT * 0.4f);
	polHeadStageClear.SetColor(255,255,255, 0);
	polHeadStageClear.SetReferenceTexture(texStrStageClear);

	polPushA.Init();
	polPushA.SetSize(256, 32);
	polPushA.SetPositionCenter(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT * 0.7f);
	polPushA.SetColor(255, 255, 255, 0);
	polPushA.SetReferenceTexture(texStrPushA);

	//----------------------------------------------------------
	// ゲームオーバー
	polHeadGameOver.Init();
	polHeadGameOver.SetSize(strWidRatio_ * headerSize_, strHeiRatio_ * headerSize_);
	polHeadGameOver.SetPositionCenter(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT * 0.3f);
	polHeadGameOver.SetColor(255,255,255,0);
	polHeadGameOver.SetReferenceTexture(texStrGameOver);

	polRestart.Init();
	polRestart.SetSize(LETTER_SIZE.x, LETTER_SIZE.y);
	polRestart.SetPositionCenter(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT * 0.7f);
	polRestart.SetColor(255, 255, 255, 0);
	polRestart.SetReferenceTexture(texRestart);

	polBackToStageSelect.Init();
	polBackToStageSelect.SetSize(LETTER_SIZE.x, LETTER_SIZE.y);
	polBackToStageSelect.SetPositionCenter(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT * 0.7f + 64);
	polBackToStageSelect.SetColor(255, 255, 255, 0);
	polBackToStageSelect.SetReferenceTexture(texBackToStageSelect);

	cursorGameOver = 0;

}

void CGameEndEffect::Update(){
	// フラグで更新を管理
	CheckGameState();

	UpdateFade();		
	UpdateInterval();
	UpdateGameClear();
	UpdateGameOver();
	UpdateGameOverCommand();
	cnt++;
}

void CGameEndEffect::Render(){
	polBlack.Render();
	polWhite.Render();
	
	RenderGameClear();
	RenderGameOver();

	RenderGameOverCommand();
}

CGameEndEffect::ECommand CGameEndEffect::GetCommand(){
	if (!isCommandSelected)
		return NOTHING;

	return command;
}

void CGameEndEffect::CheckGameState(){
	state = GAME;
	if (pGameState->isGameOver){
		state = OVER;
	}
	if (pGameState->isGameClear){	// クリアを優先
		state = CLEAR;
	}
}

void CGameEndEffect::UpdateFade(){
	if (isFadeEnd)
		return;

	int incCol = 3;	// 色の増加量

	switch (state){
		case GAME:
			polWhite.AddColor(0, 0, 0, -incCol);
			polBlack.AddColor(0, 0, 0, -incCol);
			break;

		case CLEAR:
			polWhite.AddColor(0, 0, 0, incCol);
			polBlack.AddColor(0, 0, 0, -incCol);
			break;

		case OVER:	
			polWhite.AddColor(0, 0, 0, -incCol);
			polBlack.AddColor(0, 0, 0, incCol);
			break;
	}

	// フェード終了
	if (polWhite.GetColor(CSquarePolygonData::VERTEX_0).a >= 255 && 
		polBlack.GetColor(CSquarePolygonData::VERTEX_0).a <= 0){
		isFadeEnd = true;
		cntStart = cnt;
	}
	if (polWhite.GetColor(CSquarePolygonData::VERTEX_0).a <= 0 &&
		polBlack.GetColor(CSquarePolygonData::VERTEX_0).a >= 255){
		isFadeEnd = true;
		cntStart = cnt;
	}
}

void CGameEndEffect::UpdateInterval(){
	if (!isFadeEnd)
		return;
	if (isIntervalEnd)
		return;

	int interval_ = 10;

	if (cnt > cntStart + interval_){
		isIntervalEnd = true;
		isEffectEnd = true;
	}
}

void CGameEndEffect::UpdateGameClear(){
	if (!(isEffectEnd && state == CLEAR))
		return;

	polHeadStageClear.Update();
	int col_ = rand() % 255;
	polHeadStageClear.AddColor(0, 0, 0, 5);

	polPushA.AddColor(0, 0, 0, 5);

	if (polHeadStageClear.GetColor(CSquarePolygonData::VERTEX_0).a >= 255){
		if (pRefInput1->GetDecide(TRIGGER)){
			command = BACK_TO_STAGE_SELECT;
			isCommandSelected = true;
			sndSEDecide.Play();
		}
	}
}

void CGameEndEffect::RenderGameClear(){
	if (!(isEffectEnd && state == CLEAR))
		return;

	polHeadStageClear.Render();
	polPushA.Render();
}

void CGameEndEffect::UpdateGameOver(){
	if (isGameOver)
		return;

	if (!(isFadeEnd && state == OVER))
		return;

	polHeadGameOver.Update();
	int col_ = rand() % 255;
	polHeadGameOver.AddColor(0, 0, 0, 5);
	polRestart.AddColor(0, 0, 0, 5);
	polBackToStageSelect.AddColor(0, 0, 0, 5);

	if (polHeadGameOver.GetColor(CSquarePolygonData::VERTEX_0).a >= 255 &&
		polRestart.GetColor(CSquarePolygonData::VERTEX_0).a >= 255)
		isGameOver = true;
}

void CGameEndEffect::RenderGameOver(){
	if (isGameOver)
		return;
	if (!(isFadeEnd && state == OVER))
		return;
	polHeadGameOver.Render();
	polRestart.Render();
	polBackToStageSelect.Render();
}

void CGameEndEffect::UpdateGameOverCommand(){
	if (!isGameOver)
		return;
	
	int selectNum_ = 2;

	if (pRefInput1->GetTriggerStickDown()){
		cursorGameOver++;
		sndSECursor.Play();
	}
	else if (pRefInput1->GetTriggerStickUp()){
		cursorGameOver += selectNum_ + 1;
		sndSECursor.Play();
	}
	cursorGameOver %= selectNum_;

	if (pRefInput1->GetDecide(TRIGGER)){
		switch (cursorGameOver){
			case 0:
				command = RESTART;
				break;
			case 1:
				command = BACK_TO_STAGE_SELECT;
				break;
		}
		sndSEDecide.Play();
		isCommandSelected = true;
	}

	// 色を変える
	if (cursorGameOver == 0){
		polRestart.SetColor(255, 255, 255, 255);
		polRestart.SetSize(LETTER_SIZE.x * 1.2f, LETTER_SIZE.y * 1.2f);
		polBackToStageSelect.SetColor(255, 255, 255, 64);
		polBackToStageSelect.SetSize(LETTER_SIZE.x, LETTER_SIZE.y);
	}
	else if (cursorGameOver == 1){
		polBackToStageSelect.SetColor(255, 255, 255, 255);
		polBackToStageSelect.SetSize(LETTER_SIZE.x * 1.2f, LETTER_SIZE.y * 1.2f);
		polRestart.SetColor(255, 255, 255, 64);
		polRestart.SetSize(LETTER_SIZE.x, LETTER_SIZE.y);
	}

}

void CGameEndEffect::RenderGameOverCommand(){
	if (!isGameOver)
		return;

	polHeadGameOver.Render();
	polRestart.Render();
	polBackToStageSelect.Render();
}

