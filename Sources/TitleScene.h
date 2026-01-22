//============================================================
// タイトル画面
#ifndef _INC_TITLE_SCENE_H
#define _INC_TITLE_SCENE_H

// インクルード
#include "BaseScene.h"
#include "SquarePolygonData.h"
#include "Polygon3D.h"
#include "PolygonScroll3D.h"
#include "PolygonText2D.h"
#include "Sound.h"

// クラス
class CTitleScene : public CBaseScene{
public:
	// 定数-----
	enum EScene{
		LOAD,
		INIT,
		FADE_IN,
		TITLE,
		GO_MENU,
		FADE_OUT,
	};

	enum EMenu{
		STORY,
		CHOICE,
		MENU_MAX,
	};

	// 関数-----
	void Init();
	void Destroy();
	CBaseScene* Run();

	void InitTitle();
	void InitMenu();

private:
	// 変数-----
	EScene scene;

	static const float TEXT_SIZE_X;
	static const float TEXT_SIZE_Y;

	CSound	sndBGM;
	CSound	sndSETitleEnter;
	CSound	sndSECursor;
	CSound	sndSEDecide;
	CSound	sndSECancel;


	CTexture *texLogo;	// ロゴ
	CPolygon2D logo;
	CTexture *texCoreLight;	// コアの光
	CPolygon2D coreLight[3];
	int coreAlpha[3];
	int addCoreAlpha[3];

	int addAlpha;

	CTexture *texPressAny;	// PressAny
	CPolygon2D pressAny;
	SVector2D pressAnySize;
	int alphaPressAny;
	int count;

	CTexture *texStoryMode;	// ストーリーモード
	CPolygon2D storyMode;

	CTexture *texChoiceMode;	// ステージ選択モード
	CPolygon2D choiceMode;

	CTexture *texBack[3];	// 背景
	CPolygonScroll3D back[3];

	CPolygonScroll3D backBase;

	EMenu cursor;	// カーソル位置を保存

	// 関数-----
	CBaseScene* ChangeGameScene();

	void Load();
	void Update();
	void Render();

public:
	CTitleScene();
	~CTitleScene();
};

#endif