//==========================================================
// 2Dポリゴンを扱うクラス
// キャラクターなどを作るときは、これらのクラスを継承すると楽
#ifndef _POLYGON_2D_H
#define _POLYGON_2D_H

//==========================================================
// インクルード
#include	"BaseSquarePolygon.h"

//==========================================================
// クラス
//----------------------------------------------------------
// ポリゴンに関するクラス
class CPolygon2D : public CBaseSquarePolygon {
public:
	virtual void	Render();

	// セッター
	// 2D敵に座標を指定する
	// 　サイズを設定してあることが前提。
	void	SetPositionLeftTop(float _x, float _y){ GetPCurData()->positionCenter.x = _x + GetPCurData()->size.x / 2.0f; GetPCurData()->positionCenter.y = _y + GetPCurData()->size.y / 2.0f; }
	void	SetPositionCenter(float _x, float _y){ GetPCurData()->positionCenter.x = _x; GetPCurData()->positionCenter.y = _y; GetPCurData()->positionCenter.z = 0; }
	void	SetAngleDegree(float _degZ){ GetPCurData()->angleDegree.x = 0; GetPCurData()->angleDegree.y = 0; GetPCurData()->angleDegree.z = _degZ; }

	SVector3D	CalcVertexPosition(CSquarePolygonData::EVertexID _vertex);

protected:
	void SetVertexFormat();

public:
	CPolygon2D();
	virtual ~CPolygon2D();
};

#endif