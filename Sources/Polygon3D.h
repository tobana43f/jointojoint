//==========================================================
// 3D空間で使用するポリゴンを扱うクラス
// キャラクターなどを作るときは、これらのクラスを継承すると楽
#ifndef _INC_POLYGON_3D_H
#define _INC_POLYGON_3D_H

#include	"BaseSquarePolygon.h"

class CPolygon3D : public CBaseSquarePolygon {
public:
	virtual void Render();

protected:
	void SetVertexFormat();

public:
	CPolygon3D();
	CPolygon3D(CSquarePolygonData _src) : CBaseSquarePolygon(_src){}
	virtual ~CPolygon3D();
};

#endif