#include	"titleScene.h"
#include	"TextureResourceInfo.h"	// 読み込むテクスチャラベルを参照するため
#include	"SoundResourceInfo.h"	
#include	"System.h"		// 画面サイズを参照
#include	"Allocator.h"	// 安全な解放関数
#include	"Math.h"
#include	"ControllerDefinition.h"

#include	"GameState.h"			// ゲームモードを渡すため
#include	"GameTestScene.h"		// ストーリーモード（仮）
#include	"StageSelectScene.h"	// ステージセレクト

#include	<math.h>
#include	<time.h>
#include	<stdlib.h>
#include	"AllTextureManager.h"

using namespace NSystem;
using namespace NAllocator;
using namespace NMath;
using namespace NControllerDefinition;
using namespace NAllTextureManager;

// テキストのサイズ
const float CTitleScene::TEXT_SIZE_X = 256;
const float CTitleScene::TEXT_SIZE_Y = 32;

//-----------------
// コンストラクタ
CTitleScene::CTitleScene(){
	scene = LOAD;	// シーン内遷移の初期化

	// カメラ
	pCameraOrtho = new CCameraOrthographic;	// 2
	pCameraPers = new CCameraPerspective;	// 3
}

//---------------
// デストラクタ
CTitleScene::~CTitleScene(){
	// カメラ解放
	DeleteSafely(pCameraOrtho);
	DeleteSafely(pCameraPers);

	Destroy();
}

//---------
// 初期化
void CTitleScene::Init(){
	SetIsSceneEnd(false);

	// フェードの初期化
	InitFade();

	// カメラ
	pCameraPers->SetCameraPosition(0, 0, -300);
	pCameraPers->SetUpDirection(0, 1, 0);
	pCameraPers->SetFocusPosition(0, 0, 0);
	pCameraPers->SetScreenAspectRatio(WINDOW_WIDTH / WINDOW_HEIGHT);
	pCameraPers->SetClipDistance(1, 10000);
	pCameraPers->SetViewAngleDeg(60);

	// アルファ値の初期化
	addAlpha = 2;

	// タイトルロゴ
	logo.Init();
	logo.SetSize(420, 420);
	logo.SetPositionCenter((WINDOW_WIDTH / 2), 130);
	logo.SetReferenceTexture(texLogo);
	alphaPressAny = 255;

	// ロゴの光
	for (int i = 0; i < 3; i++){
		coreAlpha[i] = (85 * i);
		addCoreAlpha[i] = 2;
	}


	SVector3D sv_ = logo.GetPositionCenter() - SVector3D(0, -35, 0);
	coreLight[0].Init();
	coreLight[0].SetSize(100, 100);
	coreLight[0].SetPositionCenter((sv_.x + 23), (sv_.y - 105));
	coreLight[0].SetReferenceTexture(texCoreLight);

	coreLight[1].Init();
	coreLight[1].SetSize(100, 100);
	coreLight[1].SetPositionCenter((sv_.x - 45), (sv_.y - 25));
	coreLight[1].SetReferenceTexture(texCoreLight);

	coreLight[2].Init();
	coreLight[2].SetSize(100, 100);
	coreLight[2].SetPositionCenter((sv_.x + 21), (sv_.y + 55));
	coreLight[2].SetReferenceTexture(texCoreLight);


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

	InitTitle();	// タイトル
	InitMenu();		// メニュー
}

void CTitleScene::InitTitle(){
	// PressAnyButtonの文字
	pressAny.Init();
	pressAny.SetColor(255, 255, 255, 255);
	pressAny.SetSize(TEXT_SIZE_X, TEXT_SIZE_Y);
	pressAny.SetPositionCenter((WINDOW_WIDTH / 2), (WINDOW_HEIGHT - 100));
	pressAny.SetReferenceTexture(texPressAny);

	count = 0;
}

void CTitleScene::InitMenu(){
	// ストーリーモード
	storyMode.Init();
	storyMode.SetColor(255, 255, 255, 255);
	storyMode.SetSize(TEXT_SIZE_X * 1.2f, TEXT_SIZE_Y * 1.2f);
	storyMode.SetPositionCenter((WINDOW_WIDTH / 2), (WINDOW_HEIGHT - 100));
	storyMode.SetReferenceTexture(texStoryMode);

	// ステージ選択モード
	choiceMode.Init();
	choiceMode.SetColor(100, 100, 100, 200);
	choiceMode.SetSize(TEXT_SIZE_X, TEXT_SIZE_Y);
	SVector3D vecter_ = storyMode.GetPositionCenter();	// 上のストーリーモードの位置をもとに計算
	choiceMode.SetPositionCenter(vecter_.x, vecter_.y + TEXT_SIZE_Y + 20);
	choiceMode.SetReferenceTexture(texChoiceMode);

	// メニューのカーソル位置を初期化
	cursor = STORY;
}

//-----------
// 終了処理
void CTitleScene::Destroy(){

}

//---------------
// シーン内遷移
CBaseScene* CTitleScene::Run(){
	switch (scene){
		case LOAD:
			Load();
			scene = INIT;

		case INIT:
			Init();
			scene = FADE_IN;
			sndBGM.Play();

		case TITLE:
		case GO_MENU:
		case FADE_IN:
		case FADE_OUT:
			Update();
			Render();
			break;
	}

	return ChangeGameScene();
}

//---------------
// シーンの移行
CBaseScene* CTitleScene::ChangeGameScene(){
	if (!GetIsSceneEnd()){
		// このシーンを続行
		return this;
	}

	sndBGM.Stop();

	// ステージ選択で開始
	CBaseScene *pNextScene = NULL;
	pNextScene = new CStageSelectScene;		// ステージセレクトに遷移

	delete this;	// タイトルを開放
	return pNextScene;
}

//---------------
// 読み込み処理
void CTitleScene::Load(){
	// テクスチャの読み込み
	// ロゴ
	texLogo = GetPTexture(NTextureResourceInfo::TITLE_LOGO);
	texCoreLight = GetPTexture(NTextureResourceInfo::CHARACTER_CORE_LIGHT);

	// 文字列
	texPressAny = GetPTexture(NTextureResourceInfo::PUSH_A_BUTTON);
	texStoryMode = GetPTexture(NTextureResourceInfo::STORY_MODE);
	texChoiceMode = GetPTexture(NTextureResourceInfo::STAGE_SELECT);

	// 背景
	switch ((rand() % 4))
	{
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

	// 音の読み込みはここで
	sndBGM.Load(NSoundResourceInfo::BGM_TITLE);
	sndSETitleEnter.Load(NSoundResourceInfo::SE_TITLE_ENTER);
	sndSECursor.Load(NSoundResourceInfo::SE_CURSOR);
	sndSEDecide.Load(NSoundResourceInfo::SE_DECIDE);
	sndSECancel.Load(NSoundResourceInfo::SE_CANCEL);
}

//-----------
// 更新処理
void CTitleScene::Update(){
	// キー操作系
	switch (scene){
		case TITLE:
			if (GetPInput(CONTROLLER_1)->GetDecide(TRIGGER)){
				scene = GO_MENU;
				sndSETitleEnter.Play();
			}
			if (alphaPressAny <= 100){
				addAlpha = 2;
			}
			else if (alphaPressAny >= 255){
				addAlpha = -2;
			}
			alphaPressAny += addAlpha;
			pressAny.SetColor(255, 255, 255, alphaPressAny);
			break;

		case GO_MENU:
			if ((count / 7) % 2){
				pressAny.SetColor(255, 255, 255, 255);
			}
			else{
				pressAny.SetColor(255, 255, 255, 0);
			}

			if (count >= 70){
				InitMenu();
				scene = FADE_OUT;
			}
			count++;
			break;

		case FADE_IN:
			if (UpdateFadeIn()){
				scene = TITLE;
			}
			break;

		case FADE_OUT:
			if (UpdateFadeOut()){
				SetIsSceneEnd(true);
			}
			break;
	}

	// 更新処理
	for (int i = 0; i < 3; i++){
		if (coreAlpha[i] >= 255){
			addCoreAlpha[i] = -2;
		}
		else if (coreAlpha[i] <= 0){
			addCoreAlpha[i] = 2;
		}
		coreAlpha[i] += addCoreAlpha[i];

		coreLight[i].SetColor(255, 255, 255, coreAlpha[i]);
	}

	backBase.Sync();
	backBase.Update();
	for (int i = 0; i < 3; i++){
		// 背景の座標をカメラと同期
		back[i].Sync();
		// スクロール
		back[i].Update();
	}

	pCameraPers->AddCameraPosition(1, 0, 0);	// カメラを動かす
	pCameraPers->AddFocusPosition(1, 0, 0);
}

//-------
// 描画
void CTitleScene::Render(){
	// 3D
	BeginRender3D();
	// 背景
	backBase.Render();
	for (int i = 0; i < 3; i++){
		back[i].Render();
	}

	// 2D
	BeginRender2D();
	// タイトルロゴ
	logo.Render();
	for (int i = 0; i < 3; i++){
		coreLight[i].Render();	// 光
	}


	// 文字たち
	switch (scene){
		// タイトル
		case TITLE:
		case GO_MENU:
			pressAny.Render();
			break;
	}

	RenderFade();
}