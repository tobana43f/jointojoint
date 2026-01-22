// インクルード
#include "StageSelectScene.h"

#include	"TextureResourceInfo.h"	// 読み込むテクスチャラベルを参照するため
#include	"SoundResourceInfo.h"	
#include	"System.h"		// 画面サイズを参照
#include	"Allocator.h"	// 安全な解放関数
#include	"Math.h"
#include	"ControllerDefinition.h"
#include	<stdlib.h>
#include	"GameState.h"

// 遷移先
#include	"GameTestScene.h"	// ゲーム
#include	"TitleScene.h"		// タイトル
#include	"AllTextureManager.h"

// ネームスペース
using namespace NSystem;
using namespace NAllocator;
using namespace NMath;
using namespace NControllerDefinition;
using namespace NAllTextureManager;

// staticの初期化
int CStageSelectScene::nowSelect = 0;
int CStageSelectScene::oldSelect = NULL;

//-----------------
// コンストラクタ
CStageSelectScene::CStageSelectScene(){
	scene = LOAD;	// シーン内遷移の初期化

	// カメラ
	pCameraOrtho = new CCameraOrthographic; // 2D
	pCameraPers = new CCameraPerspective;	// 3D

	// ステージの分割数値を設定
	split = 3;	// 今は横に３つ並ぶ

	// サイズの定義
	sizeX = (WINDOW_HEIGHT / ((split * 5.0f) + 1.0f)) * 4.0f;
	sizeY = sizeX;

	// 項目の隙間の幅
	clearanceX = (WINDOW_HEIGHT / ((split * 5.0f) + 1.0f));
	clearanceY = (WINDOW_HEIGHT / ((split * 5.0f) + 1.0f))/* * 2.0f*/;

	// ズラす幅を計算
	shift = (WINDOW_WIDTH - WINDOW_HEIGHT) / 2.0f;
}

//---------------
// デストラクタ
CStageSelectScene::~CStageSelectScene(){
	// カメラの解放
	DeleteSafely(pCameraOrtho);	// 2D
	DeleteSafely(pCameraPers);	// 3D

	Destroy();	// 終了処理
}

//---------
// 初期化
void CStageSelectScene::Init(){
	SetIsSceneEnd(false);	// シーン終了フラグの初期化

	InitFade();

	// カメラの初期化
	// カメラ
	pCameraPers->SetCameraPosition(0, 0, -300);
	pCameraPers->SetUpDirection(0, 1, 0);
	pCameraPers->SetFocusPosition(0, 0, 0);
	pCameraPers->SetScreenAspectRatio(WINDOW_WIDTH / WINDOW_HEIGHT);
	pCameraPers->SetClipDistance(1, 10000);
	pCameraPers->SetViewAngleDeg(60);

	// ステージ選択の項目の初期化-----------------
	for (int i = 0; i < CStageSelectScene::STAGE_ALL; i++){
		float x_ = 0;	// 描画座標
		float y_ = 0;
		int numX_ = 0;	// 計算用
		int numY_ = 0;

		// 全項目のサイズを設定
		icon[i].Init();
		frame[i].Init();
		lamp[i].Init();

		icon[i].SetSize(sizeX, sizeY);
		frame[i].SetSize(sizeX, sizeY);
		lamp[i].SetSize(sizeX / 1.7f, sizeY / 1.7f);

		frame[i].SetColor(255, 255, 255, 255);
		lamp[i].SetColor(255, 255, 255);

		frame[i].SetReferenceTexture(texFrame[i]);

		// ステージの座標の設定
		// チュートリルだけちょっと特別な座標
		if (i == 0){
			// チュートリアル-------------------------------------------------------------------
			// 座標計算
			x_ = WINDOW_WIDTH / 2.0f;
			y_ = clearanceY + (sizeY / 2.0f);

			// 座標の設定
			icon[i].SetPositionCenter(x_, y_);	// アイコン
			frame[i].SetPositionCenter(x_, y_);	// フレーム

			// 色の設定
			icon[i].SetColor(116, 200, 255, 255);

			// ランプのテクスチャ決定
			lamp[i].SetReferenceTexture(texLamp[1]);
		}
		else{
			// ステージ-------------------------------------------------------------------------
			// 座標計算
			numX_ = (i - 1) % split;	// 座標の計算
			numY_ = (i - 1) / split;
			x_ = (clearanceX * (numX_ + 1)) + (sizeX * numX_) + shift + (sizeX / 2.0f);
			y_ = (clearanceY * (numY_ + 2)) + (sizeY * (numY_ + 1)) + (sizeY / 2.0f);

			// 座標の設定
			icon[i].SetPositionCenter(x_, y_);
			frame[i].SetPositionCenter(x_, y_);

			// 色の設定
			switch ((i % 3)){
				case 1:
					icon[i].SetColor(50, 204, 18);
					break;

				case 2:
					icon[i].SetColor(251, 231, 49);
					break;

				case 0:
					icon[i].SetColor(239, 117, 152);
					break;
			}

			// ランプの設定
			lamp[i].SetReferenceTexture(texLamp[0]);
		}

		// ランプ位置の計算
		x_ = x_ - (sizeX / 3.25f);
		y_ = y_ + (sizeY / 3.25f);
		lamp[i].SetPositionCenter(x_, y_);
	}

	// アイコンテクスチャのセット
	icon[0].SetReferenceTexture(texIcon[0]);
	icon[1].SetReferenceTexture(texIcon[1]);
	icon[2].SetReferenceTexture(texIcon[2]);
	icon[3].SetReferenceTexture(texIcon[3]);
	icon[4].SetReferenceTexture(texIcon[4]);
	icon[5].SetReferenceTexture(texIcon[5]);
	icon[6].SetReferenceTexture(texIcon[6]);

	// 背景
	backBase.Init();
	backBase.SetPCameraPos(pCameraPers->GetCameraPPosition());
	backBase.SetOld();
	backBase.SetScrollSpeed(0);
	backBase.CalcDistance();
	backBase.SetColorToVertex(CSquarePolygonData::VERTEX_0, 0, 70, 105);
	backBase.SetColorToVertex(CSquarePolygonData::VERTEX_1, 0, 70, 105);
	backBase.SetColorToVertex(CSquarePolygonData::VERTEX_2, 40, 130, 160);
	backBase.SetColorToVertex(CSquarePolygonData::VERTEX_3, 40, 130, 160);

	for (int i = 0; i < 3; i++){
		back[i].Init();
		back[i].SetPCameraPos(pCameraPers->GetCameraPPosition());
		back[i].SetOld();
		back[i].SetReferenceTexture(texBack[i]);
		back[i].SetScrollSpeed((0.001f * (float)(i + 1)));	// スクロール速度設定
		back[i].CalcDistance();
		back[i].SetColorToVertex(CSquarePolygonData::VERTEX_0, 50, 120, 155);
		back[i].SetColorToVertex(CSquarePolygonData::VERTEX_1, 50, 120, 155);
		back[i].SetColorToVertex(CSquarePolygonData::VERTEX_2, 100, 220, 255);
		back[i].SetColorToVertex(CSquarePolygonData::VERTEX_3, 100, 220, 255);
	}
}

//-----------
// 終了処理
void CStageSelectScene::Destroy(){

}

//---------------
// シーン内遷移
CBaseScene* CStageSelectScene::Run(){
	switch (scene){
		case LOAD:
			Load();
			scene = INIT;

		case INIT:
			Init();
			scene = FADE_IN;
			sndBGM.Play();

		case MAIN:
		case FADE_IN:
		case FADE_OUT:
		case SELECT_OK:
			Update();
			Render();
			break;
	}

	return ChangeGameScene();
}

//---------------
// シーンの移行
CBaseScene* CStageSelectScene::ChangeGameScene(){
	if (!GetIsSceneEnd()){
		// シーン続行
		return this;
	}


	sndBGM.Stop();

	// シーンの移行
	CBaseScene *pNextScene_ = NULL;
	CGameState gs_;

	switch (goNext){
		// タイトルに戻る
		case GO_TITLE:
			pNextScene_ = new CTitleScene;
			break;

			// ゲームスタート
		case GO_GAME:
			gs_.Init(CGameState::STAGE_SELECT, nowSelect);
			pNextScene_ = new CGameTestScene(gs_);	// ゲームに遷移
			break;
	}

	delete this;
	return pNextScene_;	// 遷移先を返す
}

//-----------
// 読み込み
void CStageSelectScene::Load(){
	sndBGM.Load(NSoundResourceInfo::BGM_STAGE_SELECT);
	sndSECursor.Load(NSoundResourceInfo::SE_CURSOR);
	sndSEDecide.Load(NSoundResourceInfo::SE_DECIDE);
	sndSECancel.Load(NSoundResourceInfo::SE_CANCEL);

	// 背景
	switch ((rand() % 4)){
		case 0:
			texBack[0] = GetPTexture(NTextureResourceInfo::BACK_1_1);
			texBack[1] = GetPTexture(NTextureResourceInfo::BACK_1_2);
			texBack[2] = GetPTexture(NTextureResourceInfo::BACK_1_3);
			break;

		case 1:
			texBack[0] = GetPTexture(NTextureResourceInfo::BACK_2_1);
			texBack[1] = GetPTexture(NTextureResourceInfo::BACK_2_2);
			texBack[2] = GetPTexture(NTextureResourceInfo::BACK_2_3);
			break;

		case 2:
			texBack[0] = GetPTexture(NTextureResourceInfo::BACK_3_1);
			texBack[1] = GetPTexture(NTextureResourceInfo::BACK_3_2);
			texBack[2] = GetPTexture(NTextureResourceInfo::BACK_3_3);
			break;

		case 3:
			texBack[0] = GetPTexture(NTextureResourceInfo::BACK_4_1);
			texBack[1] = GetPTexture(NTextureResourceInfo::BACK_4_2);
			texBack[2] = GetPTexture(NTextureResourceInfo::BACK_4_3);
			break;

		default:
			break;
	}

	//// テストで初期化
	//for (int i = 0; i < CStageSelectScene::STAGE_ALL; i++){
	//	texIcon[i].LoadTexture(NTextureResourceInfo::TEST);
	//	texFrame[i].LoadTexture(NTextureResourceInfo::TEST);
	//}

	// フレーム
	texFrame[0] = GetPTexture(NTextureResourceInfo::SELECT_FRAME_TUTO);
	texFrame[1]= GetPTexture(NTextureResourceInfo::SELECT_FRAME_1_1);
	texFrame[2]= GetPTexture(NTextureResourceInfo::SELECT_FRAME_1_2);
	texFrame[3]= GetPTexture(NTextureResourceInfo::SELECT_FRAME_1_3);
	texFrame[4]= GetPTexture(NTextureResourceInfo::SELECT_FRAME_2_1);
	texFrame[5]= GetPTexture(NTextureResourceInfo::SELECT_FRAME_2_2);
	texFrame[6]= GetPTexture(NTextureResourceInfo::SELECT_FRAME_2_3);

	// フレームのランプ
	texLamp[1]= GetPTexture(NTextureResourceInfo::CHARACTER_CORE_ACTIVE);	// on
	texLamp[0]= GetPTexture(NTextureResourceInfo::CHARACTER_CORE_NEGATIVE);	// off

	// アイコン
	texIcon[0]= GetPTexture(NTextureResourceInfo::STAGE_ICON_TUTO);
	texIcon[1]= GetPTexture(NTextureResourceInfo::STAGE_ICON_1_1);
	texIcon[2]= GetPTexture(NTextureResourceInfo::STAGE_ICON_1_2);
	texIcon[3]= GetPTexture(NTextureResourceInfo::STAGE_ICON_1_3);
	texIcon[4]= GetPTexture(NTextureResourceInfo::STAGE_ICON_2_1);
	texIcon[5]= GetPTexture(NTextureResourceInfo::STAGE_ICON_2_2);
	texIcon[6] = GetPTexture(NTextureResourceInfo::STAGE_ICON_2_3);
}

//-----------
// 更新処理
void CStageSelectScene::Update(){
	switch (scene){
		case MAIN:
			// キー操作
			// 決定
			if (GetPInput(CONTROLLER_1)->GetDecide(TRIGGER)){
				scene = SELECT_OK;
				goNext = GO_GAME;	// ゲームへ遷移
				sndSEDecide.Play();
				
			}
			else if (GetPInput(CONTROLLER_1)->GetCancel(TRIGGER)){
				scene = SELECT_OK;
				goNext = GO_TITLE;	// タイトル
				sndSECancel.Play();
			}
			// ↓
			//else if(GetPInput(CONTROLLER_1)->GetVerticalNegative(TRIGGER))
			else if (GetPInput(CONTROLLER_1)->GetTriggerStickDown())
			{
				// チュートリアルを選択中
				if (nowSelect == 0){
					if (oldSelect == NULL){
						// １番最初、チュートリアルにいるときだけ
						nowSelect = 1;	// 1-1に移動
						sndSECursor.Play();
					}
					else{
						// チュートリアルからの移動
						nowSelect = oldSelect;	// 前いた場所に移動
						sndSECursor.Play();
					}
				}
				// 通常ステージを選択中
				else{
					MoveCursor(+split);	// 下に移動
					sndSECursor.Play();
				}
			}
			// ↑
			//else if(GetPInput(CONTROLLER_1)->GetVerticalPositive(TRIGGER))
			else if (GetPInput(CONTROLLER_1)->GetTriggerStickUp())
			{
				MoveCursor(-split);	// 上に移動
				sndSECursor.Play();
			}
			// →
			//else if(GetPInput(CONTROLLER_1)->GetHorizontalPositive(TRIGGER))
			if (GetPInput(CONTROLLER_1)->GetTriggerStickRight())
			{
				MoveCursor(+1);	// 右に移動
				sndSECursor.Play();
			}
			// ←
			//else if(GetPInput(CONTROLLER_1)->GetHorizontalNegative(TRIGGER))
			else if (GetPInput(CONTROLLER_1)->GetTriggerStickLeft())
			{
				MoveCursor(-1);	// 左に移動
				sndSECursor.Play();
			}
			break;

		case SELECT_OK:
			scene = FADE_OUT;
			break;

		case FADE_IN:
			if (UpdateFadeIn()){
				scene = MAIN;
			}
			break;

		case FADE_OUT:
			if (UpdateFadeOut()){
				SetIsSceneEnd(true);
			}
			break;
	}

	// 描画色とランプのテクスチャの入れ替え
	for (int i = 0; i < CStageSelectScene::STAGE_ALL; i++){
		// 選択中かどうか
		if (i == nowSelect){
			// 選択してる
			//icon[i].SetSize(sizeX * 1.2f, sizeY * 1.2f);	// ちょっと大きく
			//frame[i].SetSize(sizeX * 1.2f, sizeY * 1.2f);
			//lamp[i].SetSize(sizeX / 1.7, sizeY / 1.7);
			lamp[i].SetReferenceTexture(texLamp[1]);	// ランプon
		}
		else{
			// 選択してない
			lamp[i].SetReferenceTexture(texLamp[0]);	// ランプoff
		}
	}

	// 背景の更新
	backBase.Sync();
	backBase.Update();
	for (int i = 0; i < 3; i++){
		// 背景の座標をカメラと同期
		back[i].Sync();
		// スクロール
		back[i].Update();
	}
	// スクロールのためにカメラを動かす
	pCameraPers->AddCameraPosition(1, 0, 0);
	pCameraPers->AddFocusPosition(1, 0, 0);
}

//-------
// 描画
void CStageSelectScene::Render(){
	// 3D
	BeginRender3D();
	// 背景
	backBase.Render();
	for (int i = 0; i < 3; i++){
		back[i].Render();
	}

	// 2D
	BeginRender2D();
	// 全項目の描画
	for (int i = 0; i < CStageSelectScene::STAGE_ALL; i++){
		icon[i].Render();	// アイコン
		frame[i].Render();	// フレーム
		lamp[i].Render();	// ランプ
	}

	RenderFade();
}

//---------------
// カーソル移動
void CStageSelectScene::MoveCursor(int _val){
	int next_ = NULL;	// ステージ選択移動先の計算用

	// 移動先を計算
	next_ = nowSelect + _val;

	// 移動先がチュートリアル
	if (next_ <= 0){
		if (nowSelect != 0){
			UpdateOld();	// 過去位置の更新
		}
		nowSelect = 0;	// チュートリアルに移動
	}
	// 移動先にステージがあるとき
	else if (next_ < CStageSelectScene::STAGE_ALL){
		UpdateOld();		// 過去位置の更新
		nowSelect = next_;	// 移動
	}

	// 移動先にステージが無い時、なにもなし
}