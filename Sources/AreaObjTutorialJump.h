//==========================================================
// チュートリアル用のエリアオブジェクト
//----------------------------------------------------------
// 内容：
// 　エリアに入ると背景にポリゴンを描画する。

#ifndef _INC_AREA_OBJECT_TUTORIAL_JUMP_H
#define _INC_AREA_OBJECT_TUTORIAL_JUMP_H

//==========================================================
// インクルード
#include	"BaseAreaObject.h"
#include	"Polygon3D.h"
#include	"Texture.h"

//==========================================================
// クラス定義
class CAreaObjectTutorialJump : public CBaseAreaObject {
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

	static void SetRefTex(CTexture *_pJump){ pRefTexTutorialJump = _pJump; }

private:
	//==========================================================
	// 変数

	static CTexture	*pRefTexTutorialJump;

	CPolygon3D	polyBack;
	CPolygon3D	polyImage;

	int anmNum;
	int anmCnt;

	//==========================================================
	// 関数

public:
	CAreaObjectTutorialJump();
	~CAreaObjectTutorialJump();
};

#endif