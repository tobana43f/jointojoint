//=======================================
// ステージセレクト画面
#ifndef _INC__SCENE_H
#define _INC__SCENE_H

// インクルード
#include "BaseScene.h"
#include "SquarePolygonData.h"
#include "Polygon3D.h"
#include "PolygonScroll3D.h"
#include "PolygonText2D.h"
#include "Sound.h"

// クラス
class CStageSelectScene :public CBaseScene{
public:
	// 定数
	enum EScene{
		LOAD,
		INIT,
		FADE_IN,
		FADE_OUT,
		MAIN,
		SELECT_OK,
	};

	// 関数
	void Init();
	void Destroy();
	CBaseScene* Run();

	// ゲッター
	static int GetNowSelect(){ return CStageSelectScene::nowSelect; }
	static int GetOldSelect(){ return CStageSelectScene::oldSelect; }

private:
	// 定数
	enum EStage {
		STAGE_MAX = 6,	// 実装されているステージ(チュートリアルを含まない)
		STAGE_ALL,		// 実装されているステージ ＋ チュートリアル
	};

	enum ENextScene{
		GO_TITLE,
		GO_GAME,
	};

	// 変数
	EScene scene;
	int split;		// ステージ配置の分割数
	static int nowSelect;	// カーソル位置	今
	static int oldSelect;	//			１つ前
	float sizeX;	// ステージ項目のサイズ
	float sizeY;
	float clearanceX;	// 項目の間の距離
	float clearanceY;
	float shift;		// ズラす分

	ENextScene goNext;	// 遷移先の検査用

	CSound	sndBGM;
	CSound	sndSECursor;
	CSound	sndSEDecide;
	CSound	sndSECancel;

	// ステージセレクト項目のポリゴンとテクスチャ
	CTexture *texIcon[CStageSelectScene::STAGE_ALL];	// ステージアイコンのテクスチャ
	CPolygon2D icon[CStageSelectScene::STAGE_ALL];	// ステージのポリゴン
	// セレクトのフレーム
	CPolygon2D frame[CStageSelectScene::STAGE_ALL];		// ポリゴン
	CTexture *texFrame[CStageSelectScene::STAGE_ALL];	// テクスチャ
	// フレームのランプ
	CPolygon2D lamp[CStageSelectScene::STAGE_ALL];		// セレクトランプ
	CTexture *texLamp[2];	// ランプのテクスチャ, 0 Off, 1 On

	// 背景用テクスチャ
	CTexture *texBack[3];	// 背景
	CPolygonScroll3D back[3];

	CPolygonScroll3D backBase;

	// 関数
	CBaseScene* ChangeGameScene();
	void Load();
	void Update();
	void Render();

	void MoveCursor(int _val);	// カーソル位置の移動
	void UpdateOld(){ oldSelect = nowSelect; }	// カーソル位置保存

public:
	CStageSelectScene();
	~CStageSelectScene();
};

#endif