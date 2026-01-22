//==========================================================
// チュートリアル用のエリアオブジェクト
//----------------------------------------------------------
// 内容：
// 　エリアに入ると背景にポリゴンを描画する。

#ifndef _INC_AREA_OBJECT_TUTORIAL_JOINT_OBJ_H
#define _INC_AREA_OBJECT_TUTORIAL_JOINT_OBJ_H

//==========================================================
// インクルード
#include	"BaseAreaObject.h"
#include	"Polygon3D.h"
#include	"Texture.h"

//==========================================================
// クラス定義
class CAreaObjectTutorialJointObj : public CBaseAreaObject {
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

	static void SetRefTex(CTexture *_pJointObj){ pRefTexTutorialJointObj = _pJointObj; }

private:
	//==========================================================
	// 変数

	static CTexture	*pRefTexTutorialJointObj;

	CPolygon3D	polyBack;
	CPolygon3D	polyImage;

	int anmNum;
	int anmCnt;

	//==========================================================
	// 関数

public:
	CAreaObjectTutorialJointObj();
	~CAreaObjectTutorialJointObj();
};

#endif