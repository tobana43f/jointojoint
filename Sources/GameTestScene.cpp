//==========================================================
// インクルード
#define		_CRT_SECURE_NO_WARNINGS
#include	"SpringSample.h"
#include	"Allocator.h"
#include	"System.h"
#include	"ControllerDefinition.h"
#include	"CollideDetecter.h"

#include	"StageSelectScene.h"
#include	"TitleScene.h"
#include	"GameTestScene.h"
#include	"AllTextureManager.h"

#include	<stdio.h>

//==========================================================
// 名前空間
using namespace NSystem;
using namespace NControllerDefinition;
using namespace NCollideDetecter;
using namespace NAllTextureManager;

//==========================================================
// メソッド定義
CGameTestScene::CGameTestScene(CGameState _gameState)
{
	//----------------------------------------------------------
	// ゲーム情報を受け取る
	gameState = _gameState;

	scene = INIT;

	//----------------------------------------------------------
	// 使用するカメラを確保する
	pCameraOrtho = new CCameraOrthographic;
	pCameraPers = new CCameraPerspective;

	//----------------------------------------------------------
	// 剛体関係
	CRigidBody::ResisterPipeline(&rigidbodyPipeline);
	rigidbodyPipeline.SetGravity(SVector3D(0, -13.0f * (1.0f / DEFAULT_FRAME_RATE), 0));

	//----------------------------------------------------------
	// 円のあたり判定を見やすくするテクスチャを設定
	CBaseCollider::SetRefTexture(texCircleCollider);

	//----------------------------------------------------------
	// キャラクタークラスが参照する情報を設定
	CAllCharacterManager::SetRefStageObj(&stageObject);
	CAllCharacterManager::SetRefJointableObj(&jointableObject);

	//----------------------------------------------------------
	// エリアオブジェクトが参照する情報を設定
	CBaseAreaObject::SetRefCharacter(&character);

	// 繋げられるオブジェクトが参照する情報を設定
	CBaseJointableObject::SetRefGameState(&gameState);
	CBaseJointableObject::SetRefCharacter(&character);
	CBaseJointableObject::SetRefInput1(GetPInput(CONTROLLER_1));

	//----------------------------------------------------------
	// ゲームオーバー
	CGameOverManager::SetGameState(&gameState);
	CGameOverManager::SetRefInfo(&character, &stageObject, &jointableObject);

	//----------------------------------------------------------
	// カメラ操作にカメラを設定
	gameCameraManager.SetPCamera(pCameraPers);
	gameCameraManager.SetPAllCharacterManager(&character);

	//----------------------------------------------------------
	// ゲーム終了時の演出
	CGameEndEffect::SetRefInput(GetPInput(CONTROLLER_1));
	CGameEndEffect::SetRefInfo(&gameState);

	Load();
}

CGameTestScene::~CGameTestScene(){
	// カメラ解放
	DeleteSafely(pCameraOrtho);
	DeleteSafely(pCameraPers);
}

void CGameTestScene::Init(){
	//----------------------------------------------------------
	// 必須の初期化

	// カメラ初期化
	pCameraPers->SetCameraPosition(0, 0, -100);
	pCameraPers->SetUpDirection(0, 1, 0);
	pCameraPers->SetFocusPosition(0, 0, 0);
	pCameraPers->SetScreenAspectRatio(WINDOW_WIDTH / WINDOW_HEIGHT);
	pCameraPers->SetClipDistance(1, 10000);
	pCameraPers->SetViewAngleDeg(60);

	// タイトル用ポリゴン初期化
	polyTextTitle.Init();
	polyTextTitle.SetReferenceTexture(texFont);
	polyTextTitle.SetSize(15, 20);
	polyTextTitle.SetPositionLeftTop(0, 0);

	// デバッグ表示用ポリゴンを初期化
	polyTextDebug.Init();
	polyTextDebug.SetReferenceTexture(texFont);
	polyTextDebug.SetSize(12, 16);

	//----------------------------------------------------------
	// キャラクターの初期化

	character.Init();

	// コントローラーのポインタを渡す
	character.SetRefInput1(GetPInput(CONTROLLER_1) );

	// 接続時のエフェクト
	pol3JointEffect.Init();
	pol3JointEffect.SetSize(30, 30);
	//pol3JointEffect.SetPositionCente

	//----------------------------------------------------------
	// ステージオブジェクト
	stageObject.Init();

	//----------------------------------------------------------
	// エリアオブジェクト
	areaObject.Init();

	//----------------------------------------------------------
	// 繋げられるオブジェクト
	jointableObject.Init();

	//----------------------------------------------------------
	// ゲームオーバー
	gameArea.Init();

	//----------------------------------------------------------
	// 背景
	int darkness_ = gameState.stageNo * 15;
	
	int tr = 50 - darkness_;
	int tg = 120 - darkness_;
	int tb = 155 - darkness_;

	int adjust = 30;
	int br = 100 - darkness_ + adjust;
	int bg = 220 - darkness_ + adjust;
	int bb = 255 - darkness_ + adjust;

	for (int i = 0; i < 3; ++i){
		back[i].Init();
		back[i].SetReferenceTexture(texBack[i]);
		back[i].SetPCameraPos(pCameraPers->GetCameraPPosition());
		back[i].SetOld();
		back[i].SetScrollSpeed((0.001f * (float)(i + 1)));	// スクロール速度設定
		back[i].CalcDistance();


		back[i].SetColorToVertex(CSquarePolygonData::VERTEX_0, tr, tg, tb);
		back[i].SetColorToVertex(CSquarePolygonData::VERTEX_1, tr, tg, tb);
		back[i].SetColorToVertex(CSquarePolygonData::VERTEX_2, br, bg, bb);
		back[i].SetColorToVertex(CSquarePolygonData::VERTEX_3, br, bg, bb);
	}

	tr += 5 * gameState.stageNo * gameState.stageNo;
	tg -= 10 * gameState.stageNo;
	tb -= 10 * gameState.stageNo;

	br -= 20 * gameState.stageNo;
	bg -= 30 * gameState.stageNo;
	bb -= 30 * gameState.stageNo;

	backBase.Init();
	backBase.SetPCameraPos(pCameraPers->GetCameraPPosition());
	backBase.SetOld();
	backBase.SetScrollSpeed(0.001f);
	backBase.CalcDistance();
	backBase.SetColorToVertex(CSquarePolygonData::VERTEX_0, tr, tg, tb);
	backBase.SetColorToVertex(CSquarePolygonData::VERTEX_1, tr, tg, tb);
	backBase.SetColorToVertex(CSquarePolygonData::VERTEX_2, br, bg, bb);
	backBase.SetColorToVertex(CSquarePolygonData::VERTEX_3, br, bg, bb);

	//----------------------------------------------------------
	gameEndEffect.Init();

	//----------------------------------------------------------
	// フェード
	InitFade();

	scene = FADE_IN;
}

void CGameTestScene::Destroy(){

}

CBaseScene* CGameTestScene::Run(){
	switch (scene){
		case INIT:
			Init();
			sndBGM.Play();

		case FADE_IN:
			if (UpdateFadeIn())
				scene = GAME;
			Update();
			Render();
			RenderFade();
			break;

		case GAME:
			Update();
			Render();
			break;

		case GAME_END:
			// 終了後のカーソル操作など
			gameEndEffect.Update();

			{	// 選択されているか判断
				CGameEndEffect::ECommand c_ = gameEndEffect.GetCommand();
				if (c_ != CGameEndEffect::NOTHING)
					scene = FADE_OUT;
			}

			// 終了後の描画
			BeginRender2D();
			gameEndEffect.Render();
			break;
			
		case FADE_OUT:
			if (UpdateFadeOut())
				SetIsSceneEnd(true);

			BeginRender2D();
			gameEndEffect.Render();
			RenderFade();
			break;
	}

	//RenderDebug();

	return ChangeGameScene();
}

CBaseScene* CGameTestScene::ChangeGameScene(){
	if (!GetIsSceneEnd())
		return this;
	
	// 遷移
	sndBGM.Stop();

	CGameEndEffect::ECommand c_ = gameEndEffect.GetCommand();
	// 初期化するだけの場合
	if (c_ == CGameEndEffect::RESTART){
		scene = INIT;
		SetIsSceneEnd(false);
		return this;
	}

	// シーンの変更
	delete this;
	// 変更時
	switch (c_){
		case CGameEndEffect::BACK_TO_STAGE_SELECT:
			return new CStageSelectScene;
			break;

		case CGameEndEffect::BACK_TO_TITLE:
			return new CTitleScene;
			break;

		case CGameEndEffect::NEXT_STAGE:
			
			break;
	}

	return new CTitleScene;
}

void CGameTestScene::Load(){
	texCircleCollider = GetPTexture(NTextureResourceInfo::CIRCLE_COLLIDER);
	texFont = GetPTexture(NTextureResourceInfo::FONT_DOT_AND_SHADOW);

	LoadStageBackTexture();

	character.Load(gameState.stageNo);

	stageObject.Load(gameState.stageNo);
	areaObject.Load(gameState.stageNo);
	jointableObject.Load(gameState.stageNo);

	gameEndEffect.Load();
}

// サウンドもついでにロードする
void CGameTestScene::LoadStageBackTexture(){
	// 背景
	switch (gameState.stageNo)
	//switch (rand() % 4)
	{
		case 0:
			texBack[0] = GetPTexture(NTextureResourceInfo::BACK_3_1);
			texBack[1] = GetPTexture(NTextureResourceInfo::BACK_3_2);
			texBack[2] = GetPTexture(NTextureResourceInfo::BACK_3_3);

			sndBGM.Load(NSoundResourceInfo::BGM_STAGE_1_0);
			break;

		case 1:
			texBack[0] = GetPTexture(NTextureResourceInfo::BACK_2_1);
			texBack[1] = GetPTexture(NTextureResourceInfo::BACK_2_2);
			texBack[2] = GetPTexture(NTextureResourceInfo::BACK_2_3);

			sndBGM.Load(NSoundResourceInfo::BGM_STAGE_1_1);
			break;

		case 2:
			texBack[0] = GetPTexture(NTextureResourceInfo::BACK_1_1);
			texBack[1] = GetPTexture(NTextureResourceInfo::BACK_1_2);
			texBack[2] = GetPTexture(NTextureResourceInfo::BACK_1_3);
			sndBGM.Load(NSoundResourceInfo::BGM_STAGE_1_2);
			break;

		case 3:
			texBack[0] = GetPTexture(NTextureResourceInfo::BACK_4_1);
			texBack[1] = GetPTexture(NTextureResourceInfo::BACK_4_2);
			texBack[2] = GetPTexture(NTextureResourceInfo::BACK_4_3);
			sndBGM.Load(NSoundResourceInfo::BGM_STAGE_1_3);
			break;

		case 4:
			texBack[0] = GetPTexture(NTextureResourceInfo::BACK_3_1);
			texBack[1] = GetPTexture(NTextureResourceInfo::BACK_2_2);
			texBack[2] = GetPTexture(NTextureResourceInfo::BACK_1_3);
			sndBGM.Load(NSoundResourceInfo::BGM_STAGE_2_1);
			break;

		case 5:
			texBack[0] = GetPTexture(NTextureResourceInfo::BACK_1_1);
			texBack[1] = GetPTexture(NTextureResourceInfo::BACK_1_2);
			texBack[2] = GetPTexture(NTextureResourceInfo::BACK_3_3);
			sndBGM.Load(NSoundResourceInfo::BGM_STAGE_2_2);
			break;

		case 6:
			texBack[0] = GetPTexture(NTextureResourceInfo::BACK_4_1);
			texBack[1] = GetPTexture(NTextureResourceInfo::BACK_2_2);
			texBack[2] = GetPTexture(NTextureResourceInfo::BACK_4_3);
			sndBGM.Load(NSoundResourceInfo::BGM_STAGE_2_3);
			break;

		default:
			texBack[0] = GetPTexture(NTextureResourceInfo::BACK_3_1);
			texBack[1] = GetPTexture(NTextureResourceInfo::BACK_3_2);
			texBack[2] = GetPTexture(NTextureResourceInfo::BACK_3_3);
			sndBGM.Load(NSoundResourceInfo::BGM_STAGE_1_0);
			break;
	}

}

//----------------------------------------------------------
// メイン

void CGameTestScene::Update(){
	//----------------------------------------------------------
	// 更新処理
	
	// カメラ更新
	gameCameraManager.Update();

	// オブジェクトの更新
	character.Update();
	stageObject.Update();
	areaObject.Update();
	jointableObject.Update();
	gameArea.Update();

	// 背景
	backBase.Sync();
	backBase.Update();
	for (int i = 0; i < 3; ++i){
		back[i].Sync();
		back[i].Update();
	}

	// 剛体シミュレーションパイプライン
	rigidbodyPipeline.Update();

	// フェード
	gameEndEffect.Update();
	
	//----------------------------------------------------------
	// 同期処理

	character.Sync();
	stageObject.Sync();
	areaObject.Sync();
	jointableObject.Sync();

	//----------------------------------------------------------
	// 画面遷移させる
	//if (GetPInput(CONTROLLER_1)->GetJump(TRIGGER)){
	//	SetIsSceneEnd(true);
	//}

	if (gameEndEffect.GetIsFadeEnd())
		scene = GAME_END;

}

void CGameTestScene::Render(){
	BeginRender3D();

	// 背景
	backBase.Render();
	for (int i = 0; i < 3; ++i)
		back[i].Render();

	areaObject.Render();
	character.Render();
	stageObject.Render();
	jointableObject.Render();
	gameArea.Render();

	BeginRender2D();
	gameEndEffect.Render();
}

//----------------------------------------------------------
// ユーティリティ

void CGameTestScene::RenderDebug(){
	BeginRender2D();
	// タイトル
	polyTextTitle.Render("GAME TEST SCENE");

	// デバッグ
	SVector2D	basePos_(10, 25);
	polyTextDebug.SetPositionLeftTop(basePos_.x, basePos_.y);

	// ゲーム情報
	{
		char buf[100] = "TEST";
		if (gameState.gameMode == CGameState::STORY)
			sprintf(buf, "STORY");
		else if (gameState.gameMode == CGameState::STAGE_SELECT)
			sprintf(buf, "STAGE SELECT");

		polyTextDebug.Render("MODE : %s", buf);
		polyTextDebug.MoveToNewLine();

		polyTextDebug.Render("STAGE NO : %d", gameState.stageNo);
		polyTextDebug.MoveToNewLine();

		sprintf(buf, "FALSE");
		if (gameState.isGameClear){ sprintf(buf, "TRUE"); }

		polyTextDebug.Render("IS GAME CLEAR : %s", buf);
		polyTextDebug.MoveToNewLine();

		sprintf(buf, "FALSE");
		if (gameState.isGameOver){ sprintf(buf, "TRUE"); }

		polyTextDebug.Render("IS GAME OVER : %s", buf);
		polyTextDebug.MoveToNewLine();

	}

	polyTextDebug.MoveToNewLine();
	{
		// キャラクター座標
		char posFormat_[] = "%s : X = %.1f, Y = %.1f";

		SVector3D p_ = character.GetPActiveCharManager()->GetPCharacterListTail()->body.GetPositionCenter();
		SVector3D v_ = character.GetPActiveCharManager()->GetPCharacterListTail()->body.GetVelocity();
		SVector3D a_ = character.GetPActiveCharManager()->GetPCharacterListTail()->body.GetAcceleration();
		polyTextDebug.Render(posFormat_, "POS", p_.x, p_.y);
		polyTextDebug.MoveToNewLine();
		polyTextDebug.Render(posFormat_, "VEL", v_.x, v_.y);
		polyTextDebug.MoveToNewLine();
		polyTextDebug.Render(posFormat_, "ACC", a_.x, a_.y);
		polyTextDebug.MoveToNewLine();

	}

}
