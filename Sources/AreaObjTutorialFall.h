//==========================================================
// チュートリアル用のエリアオブジェクト
//----------------------------------------------------------
// 内容：
// 　エリアに入ると背景にポリゴンを描画する。

#ifndef _INC_AREA_OBJECT_TUTORIAL_FALL_H
#define _INC_AREA_OBJECT_TUTORIAL_FALL_H

//==========================================================
// インクルード
#include	"BaseAreaObject.h"
#include	"Polygon3D.h"
#include	"Texture.h"

//==========================================================
// クラス定義
class CAreaObjectTutorialFall : public CBaseAreaObject {
public:
	//==========================================================
	// 定数
	int GetFlipSpeed(){ return (int)(60 * 1.0f); }

	//==========================================================
	// 関数

	void Init(SVector3D _initPos);
	void Update();
	void Sync();
	void Render();

	static void SetRefTex(CTexture *_pFall){ pRefTexTutorialFall = _pFall; }

private:
	//==========================================================
	// 変数

	static CTexture	*pRefTexTutorialFall;

	CPolygon3D	polyBack;
	CPolygon3D	polyImage;

	int anmNum;
	int anmCnt;

	//==========================================================
	// 関数

public:
	CAreaObjectTutorialFall();
	~CAreaObjectTutorialFall();
};

#endif