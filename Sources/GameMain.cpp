//==========================================================
// インクルード
#include	"GameMain.h"
#include	"System.h"	
#include	"Allocator.h"	// 安全な解放

#include	"TitleScene.h"
#include	"SpringSample.h"
#include	"GameTestScene.h"
#include	"PendulumScene.h"
#include	"RigidbodyScene.h"

#include	<stdlib.h>
#include	<time.h>	// srand()の初期化

#include	"AllTextureManager.h"

//==========================================================
// 名前空間

using namespace NControllerDefinition;
using namespace NAllocator;
using namespace NAllTextureManager;


//==========================================================
// メソッド定義

CGameMain::CGameMain(){
	// コントローラーを確保
	AllocController();

	// シーンで使わせるコントローラーを渡す
	CBaseScene::SetRefController(pInput);

	// 全テクスチャをロード
	LoadAllTexture();

	// その他初期化
	pCurScene = NULL;
}

CGameMain::~CGameMain(){
	// コントローラーを解放
	FreeController();
}

void CGameMain::Init(){
	// システム関係の初期化
	srand((unsigned int)time(NULL));

	// コントローラーの初期化
	InitController();

	// 一番最初に使用するシーンを確保する
	pCurScene = new CTitleScene;
	//pCurScene = new CSpringSampleScene;
	//pCurScene = new CPendulumScene;

	//CGameState	gs_;
	//gs_.Init(CGameState::TEST, 0);
	//pCurScene = new CGameTestScene(gs_);
}

void CGameMain::Destroy(){
	// シーンを解放する
	DeleteSafely(pCurScene);
}

void CGameMain::Run(){
	Update();
	pCurScene = pCurScene->Run();
	Render();
}

void CGameMain::Update(){
	// プラットフォームの生入力データを更新(ここでのみ呼び出す。)
	CInput::UpdatePlatformInput();

	// コントローラーを更新
	UpdateController();
}

void CGameMain::Render(){

}


void CGameMain::AllocController(){
	for (int i = 0; i < CONTROLLER_MAX; ++i){
		pInput[i] = new CInput;
	}
}

void CGameMain::FreeController(){
	for (int i = 0; i < CONTROLLER_MAX; ++i){
		DeleteSafely(pInput[i]);
	}
}

void CGameMain::InitController(){
	for (int i = 0; i < CONTROLLER_MAX; ++i)
		pInput[i]->Init((EControllerNo)i);	// それぞれのコントローラー番号を割り当てる

}

void CGameMain::UpdateController(){
	for (int i = 0; i < CONTROLLER_MAX; ++i)
		pInput[i]->Update();
}