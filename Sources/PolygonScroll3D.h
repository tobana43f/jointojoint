#ifndef _INC_POLYGONSCROLL3D_H
#define _INC_POLYGONSCROLL3D_H

#include "Polygon3D.h"	// 継承
#include "MathTypeDefinition.h"	// SVector3D

// クラス
class CPolygonScroll3D : public CPolygon3D{
public:
	//-------
	// 関数
	void Render();			// 描画
	void Update();			// 更新（アニメーション、スクロールなど）
	void Sync();			// カメラと座標を同期（シーンのUpdateの最後で呼び出し）
	void CalcDistance();	// カメラとの距離を１度だけ計算
	void InitLast();

	// セッター
	//-----------------------------------
	// カメラのポインタを渡す
	// 初期化の段階で１度だけ呼び出す
	void SetPCameraPos(SVector3D *_pCameraPos){ pCameraPos = _pCameraPos; }
	//-------------------------------------------------
	// スクロール量を設定
	// カメラの移動量につきスクロールする量の設定
	// 初期化の段階で呼びだす
	void SetScrollSpeed(float _n){ speed = _n; }

	void SetPositionCenter(float _x, float _y){ GetPCurData()->positionCenter.x = _x; GetPCurData()->positionCenter.y = _y; }
	void SetOld(){ old.x = pCameraPos->x; }	// カメラ位置の過去情報を登録

	// ゲッター

private:
	//-------
	// 変数
	SVector3D *pCameraPos;	// カメラの座標保存用
	SVector3D oldCameraPos;	// 1フレーム前のカメラの位置
	float distance;			// カメラとポリゴンの距離
	SVector2D scroll;		// スクロール分の数値
	float speed;			// スクロール速度・カメラが１移動するたびにスクロールする量
	SVector3D moved;		// カメラの移動した量
	SVector3D old;			// カメラの前の座標

	//-------
	// 関数
	void CalcMove();	// カメラがどれだけ動いたかを計算
	void CalcSpeed();	// スクロール速度を計算
	void Scroll();		// スクロール実行

public:
	CPolygonScroll3D();
	~CPolygonScroll3D();
};


#endif