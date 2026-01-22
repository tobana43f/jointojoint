//==========================================================
// インクルード

#include	"BaseScene.h"	// 定義
#include	"System.h"		// 画面サイズを参照

using namespace NSystem;


//==========================================================
// 名前空間

using namespace NControllerDefinition;

//==========================================================
// 静的領域

CInput	*CBaseScene::pInput[CONTROLLER_MAX];

//==========================================================
// メソッド定義

CBaseScene::CBaseScene(){
	SetIsSceneEnd(false);
	
	//InitMyMemory();
}

CBaseScene::~CBaseScene(){

}

void CBaseScene::SetRefController(CInput *_pInput[CONTROLLER_MAX]){
	for (int i = 0; i < CONTROLLER_MAX; ++i){
		pInput[i] = _pInput[i];
	}
}

CInput*	CBaseScene::GetPInput(EControllerNo _no){
	if (_no == CONTROLLER_MAX){
		// error
		return pInput[0];	// 適当なコントローラーを返しておく(エラー)
	}
	return pInput[_no];
}

//--------------------------------------------------------------------------------------------
// フェードインアウト
// 初期化
void CBaseScene::InitFade(){
	fadeAlpha = 255;	// アルファ値の初期化

	// ポリゴンの初期化
	fade.Init();
	fade.SetSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	fade.SetPositionLeftTop(0, 0);
	fade.SetColor(255, 255, 255, fadeAlpha);
}

// 更新処理
// イン
bool CBaseScene::UpdateFadeIn(){
	fadeAlpha -= 5;	// 透明に
	fade.SetColor(255, 255, 255, (fadeAlpha % 256));
	if (fadeAlpha <= 0){
		return true;	// フェードイン終わり
	}
	return false;	// フェードイン中
}

// アウト
bool CBaseScene::UpdateFadeOut(){
	fadeAlpha += 5;	// 不透明に
	fade.SetColor(255, 255, 255, (fadeAlpha % 256));
	if (fadeAlpha >= 255){
		return true;			// フェードアウト終わり
	}
	return false;	// フェードアウト中
}

// 描画
void CBaseScene::RenderFade(){
	fade.Render();	// 描画
}

// フェードインアウト・ここまで
//----------------------------------------------------------------------------------------------