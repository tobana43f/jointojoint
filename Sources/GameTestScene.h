//==========================================================
// ゲーム画面のサンプルを作成
#ifndef _INC_GAME_TEST_H
#define _INC_GAME_TEST_H

//==========================================================
// インクルード
#include	"BaseScene.h"
#include	"PolygonText2D.h"
#include	"PolygonScroll3D.h"
#include	"Texture.h"
#include	"Sound.h"

#include	"AllCharacterManager.h"
#include	"AllStageObjectManager.h"
#include	"AllAreaObjectManager.h"
#include	"AllJointableObjectManager.h"
#include	"GameOverManager.h"
#include	"RigidbodyPairManager.h"
#include	"GameCameraManager.h"
#include	"GameState.h"

#include	"RigidbodyPipeline.h"

#include	"GameEndEffect.h"

//==========================================================
// クラス定義
class CGameTestScene : public CBaseScene {
public:
	//==========================================================
	// 定数

	enum EScene {	// クラス内遷移
		INIT,
		FADE_IN,
		GAME,
		GAME_END,
		FADE_OUT,
	};

	//==========================================================
	// 関数

	void Init();
	void Destroy();
	CBaseScene* Run();

private:
	//==========================================================
	// 変数

	//----------------------------------------------------------
	// 共通

	EScene		scene;

	// ゲーム情報
	CGameState	gameState;

	// リソース
	CTexture	*texCircleCollider;		// あたり判定用
	CTexture	*texFont;				// 文字列描画用
	CTexture	*texBack[3];				// 背景１

	// サウンド
	CSound		sndBGM;

	// デバッグ関係
	CPolygonText2D	polyTextTitle;
	CPolygonText2D	polyTextDebug;			// 値を見るための

	//----------------------------------------------------------
	// ゲームに必要なもの

	// キャラクター
	CAllCharacterManager	character;
	CPolygon3D				pol3JointEffect;

	// ステージオブジェクト
	CAllStageObjectManager	stageObject;

	// エリアオブジェクト
	CAllAreaObjectManager	areaObject;

	// 繋げられるオブジェクト
	CAllJointableObjectManager	jointableObject;

	// ゲームオーバー(死亡するエリアを管理する)
	CGameOverManager	gameArea;

	// 背景
	CPolygonScroll3D		backBase;	// 一番後ろ
	CPolygonScroll3D		back[3];

	// 剛体を処理するパイプライン
	CRigidbodyPipeline	rigidbodyPipeline;

	// カメラの操作
	CGameCameraManager	gameCameraManager;

	//----------------------------------------------------------
	// その他演出用

	CGameEndEffect	gameEndEffect;

	//==========================================================
	// 関数

	void Load();

	// メイン
	void Update();
	void Render();

	CBaseScene* ChangeGameScene();

	// デバッグ情報の描画
	void RenderDebug();

	// ステージの背景テクスチャをステージ番号によって読み込む
	void LoadStageBackTexture();

public:
	CGameTestScene(CGameState _gameState);
	~CGameTestScene();
};

#endif