//==========================================================
// あたり判定情報クラスの抽象クラス
#ifndef _INC_BASE_COLLIDER_H
#define _INC_BASE_COLLIDER_H

//==========================================================
// インクルード
#include	"MathTypeDefinition.h"
#include	"Polygon3D.h"
#include	"Texture.h"	

//==========================================================
// 定義
#undef PURE_FUNC
#define PURE_FUNC =0

//==========================================================
// クラス定義
class CBaseCollider {
public:
	//==========================================================
	// 関数

	// 必ずオーナーを内包する形にサイズを更新すること。
	virtual void Update();

	// あたり判定ポリゴンの座標の同期が主な処理
	virtual void Sync();

	// あたり判定用ポリゴンを描画できるようにする
	virtual void Render();

	// 円のあたり判定を確認するためのテクスチャを渡す。
	// シーンの最初に呼び出すこと。
	static void SetRefTexture(CTexture *_pTexCircle){ pTexRefCircle = _pTexCircle; }

	// あたり判定の描画色を設定する
	// メモ)
	// 　デフォルトで、半透明にしてある。
	void SetColor(int _r, int _g, int _b, int _a = 128){ polyCollider.SetColor(_r, _g, _b, _a); }

	// あたり判定の有効無効フラグを変更する
	void SetIsActive(bool _state){ isActive = _state; }

	// あたり判定を所持するオブジェクトの座標のポインタを設定
	void SetRefOwner(CPolygon3D *_pOwner){ pOwnerPolygon3D = _pOwner; }

	// あたり判定座標を設定
	void SetRelativePositionCenter(SVector3D _posCenter){ relativePosition = _posCenter; }

	// あたり判定の有効無効フラグを取得
	bool GetIsActive(){ return isActive; }

	// あたり判定の絶対座標を取得する
	// オーナーが設定されていない場合は相対座標を絶対座標として返す。
	SVector3D GetWorldPositionCenter();

protected:
	//==========================================================
	// 変数

	CPolygon3D*		GetPOwner(){ return pOwnerPolygon3D; }

	// デバッグ用
	static CTexture *pTexRefCircle;		// 円のあたり判定を確認できるようにするテクスチャ
	CPolygon3D		polyCollider;		// あたり判定を描画するポリゴン

private:
	//==========================================================
	// 変数

	// あたり判定の有効無効フラグ
	bool		isActive;

	// あたり判定を所持しているオブジェクトの中心座標のポインタ
	CPolygon3D	*pOwnerPolygon3D;

	// オーナーの座標からのあたり判定の相対座標
	SVector3D	relativePosition;


public:
	CBaseCollider();
	virtual ~CBaseCollider();
};

#endif