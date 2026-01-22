//==========================================================
// ポリゴンを使用した線描がクラス
// 始点と終点を決め、その間を四角形ポリゴンで描画する。
// メモ)
// 　頂点と配列の対応(TRIANGLE_FAN)
// 　 01 
// 　 32
// メモ)
// 　2D上では正面を向く
#ifndef _INC_POLYGON_LINE_H
#define _INC_POLYGON_LINE_H

//==========================================================
// インクルード

#include	"MathTypeDefinition.h"	// 両方に対応した型の定義を参照
#include	"Texture.h"

//==========================================================
// クラス定義

class CPolygonLine {
public:
	//==========================================================
	// 定数

	// 頂点の指定
	enum EVertexID{
		VERTEX_0,	// 左上
		VERTEX_1,	// 右上
		VERTEX_2,	// 右下
		VERTEX_3,	// 左下
		VERTEX_MAX
	};

	// ブレンドタイプを指定
	enum EBlendType{	// 描画時の合成方法
		REPLACE,		// 上書き(通常の合成方法)
		ADD,			// 加算合成
		SUB,			// 減算合成
	};

	//==========================================================
	// 関数

	void Init();
	void Destroy();
	void Update();
	void Render();
	
	// 値に加算する関数
	void	AddColor(int _r, int _g, int _b, int _a = 0);
	void	AddColorToVertex(EVertexID _vxId, int _r, int _g, int _b, int _a = 0);

	// セッター
	void	SetPositionStart(float _x, float _y, float _z){ positionStart.x = _x; positionStart.y = _y; positionStart.z = _z; }
	void	SetPositionEnd(float _x, float _y, float _z){ positionEnd.x = _x; positionEnd.y = _y; positionEnd.z = _z; }
	void	SetLineWidth(float _width){ lineWidth = _width; }

	void	SetColor(int _r, int _g, int _b, int _a = 255);
	void	SetColorToVertex(EVertexID _vxId, int _r, int _g, int _b, int _a = 255);
	void	SetReferenceTexture(CTexture *_pTexture){ pTexture = _pTexture; }

	void	SetAnmNo(int _anmNo);
	void	SetIsReverseX(bool _state){ isReverseX = _state; }
	void	SetIsReverseY(bool _state){ isReverseY = _state; }
	void	SetBlendType(EBlendType _type){ blendType = _type; }

	// ゲッター
	SVector3D	GetPositionStart(){ return positionStart; }
	SVector3D	GetPositionEnd(){ return positionEnd; }
	float		GetLineWidth(){ return lineWidth; }
	CTexture*	GetPTexture(){ return pTexture; }
	SColor		GetColor(EVertexID _vertex){ return color[_vertex]; }

	bool		GetIsAnmEnd(){ return isAnmEnd; }

	SVector3D	CalcVertexPosition(int _vertex);

private:
	//==========================================================
	// 変数

	SVector3D	positionStart;			// 始点
	SVector3D	positionEnd;			// 終点
	float		lineWidth;				// 線の太さ

	SColor		color[VERTEX_MAX];		// 頂点カラー

	CTexture	*pTexture;				// テクスチャ情報のポインタを格納する
	int			anmCnt;					// アニメーションカウンタを指定
	int			anmNo;					// アニメーションを指定
	bool		isAnmEnd;				// アニメーションの終了フラグ　true:終了(ANM_END) false:更新中
	bool		isReverseX;				// x方向反転フラグ	true:左向(反対方向) false:右向(正方向) 
	bool		isReverseY;				// y方向反転フラグ　 true:下(反転)　false:上(正方向)

	EBlendType	blendType;				// 描画時の合成方法の指定

	//==========================================================
	// 関数

	// ユーティリティ
	void		UpdateAnimation();

	void		SetVertexFormat();		// 頂点フォーマットの指定(継承先で自由なフォーマットを指定)
	void		SetTexture();			// 描画前に実際にテクスチャをセットする
	void		SetRenderState();		// レンダーステートの指定(継承先で自由なフォーマットを指定)

	void		SetRenderStateReplace();		// 通常の合成
	void		SetRenderStateAdd();			// 加算合成をするときのレンダーステート
	void		SetRenderStateSub();			// 減算合成

	SCoord2D	GetTextureNo();			// アニメーション用連番テクスチャの番号の取得
	SCoord2D	GetTextureNoOffset();	// アニメーション用連番テクスチャの上下左右の場所を調整する
	SVector2D	GetTextureDivSizeUV();	// 分割テクスチャの1つあたりのUV座標のサイズを取得


public:
	CPolygonLine();
	~CPolygonLine();

};

#endif