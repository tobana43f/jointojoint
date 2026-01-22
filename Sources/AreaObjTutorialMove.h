//==========================================================
// チュートリアル用のエリアオブジェクト
//----------------------------------------------------------
// 内容：
// 　エリアに入ると背景にポリゴンを描画する。

#ifndef _INC_AREA_OBJECT_TUTORIAL_MOVE_H
#define _INC_AREA_OBJECT_TUTORIAL_MOVE_H

//==========================================================
// インクルード
#include	"BaseAreaObject.h"
#include	"Polygon3D.h"
#include	"Texture.h"

//==========================================================
// クラス定義
class CAreaObjectTutorialMove : public CBaseAreaObject {
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

	static void SetRefTex(CTexture *_pMove){ pRefTexTutorialMove = _pMove; }

private:
	//==========================================================
	// 変数

	static CTexture	*pRefTexTutorialMove;

	CPolygon3D	polyBack;
	CPolygon3D	polyImage;

	int anmNum;
	int anmCnt;

	//==========================================================
	// 関数

public:
	CAreaObjectTutorialMove();
	~CAreaObjectTutorialMove();
};

#endif