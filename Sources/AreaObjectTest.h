//==========================================================
// テスト用のエリアオブジェクト
//----------------------------------------------------------
// 内容：
// 　エリアに入ると背景にポリゴンを描画する。

#ifndef _INC_AREA_OBJECT_TEST_H
#define _INC_AREA_OBJECT_TEST_H

//==========================================================
// インクルード
#include	"BaseAreaObject.h"
#include	"Polygon3D.h"
#include	"Texture.h"

//==========================================================
// クラス定義
class CAreaObjectTest : public CBaseAreaObject {
public:
	//==========================================================
	// 定数

	//==========================================================
	// 関数

	void Init(SVector3D _initPos);
	void Update();
	void Sync();
	void Render();

private:
	//==========================================================
	// 変数

	CPolygon3D	polyBack;

	//==========================================================
	// 関数	

public:
	CAreaObjectTest();
	~CAreaObjectTest();
};

#endif