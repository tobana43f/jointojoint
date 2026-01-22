//==========================================================
// ビットマップフォントテクスチャを使用した、文字列描画を行うクラス。
// 2D専用
// 頂点の描画順は、SquarePolygonDataと同じ
// メモ)
// 　ビットマップテクスチャを作成する際は、" " (半角空白)から始めた、ASCIIコード表に対応させること。
// 　1番左上から順に並べ、すべて等間隔で配置すること。
// 　改行(\n)等は使えない。
#ifndef _INC_POLYGON_TEXT_2D_H
#define _INC_POLYGON_TEXT_2D_H

#include	"MathTypeDefinition.h"	// ベクトル型を使用する
#include	"Texture.h"					// テクスチャを使う

class CPolygonText2D {
public:
	// 定数
	enum EVertexID{
		VERTEX_0,	// 左上
		VERTEX_1,	// 右上
		VERTEX_2,	// 右下
		VERTEX_3,	// 左下
		VERTEX_MAX
	};

	// 基本的な関数
	void	Init();
	void	Destroy();
	void	Render(const char *_format, ...);	// 使い慣れた printf() のように呼び出せるようにする。

	// 値に加算する関数(移動させるときなどに便利)
	void	AddPositionLeftTop(float _x, float _y){ positionLeftTop.x += _x; positionLeftTop.y += _y; }
	void	AddSize(float _w, float _h){ sizeLetter.x += _w; sizeLetter.y += _h; }
	void	AddColor(unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a = 0);
	void	AddColorToVertex(EVertexID _vxId, unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a = 0);

	// セッター系
	void	SetPositionLeftTop(float _x, float _y){ positionLeftTop.x = _x; positionLeftTop.y = _y; }
	void	SetSize(float _w, float _h){ sizeLetter.x = _w; sizeLetter.y = _h; }
	void	SetColor(unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a = 255);
	void	SetColorToVertex(EVertexID _vxId, unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a = 255);

	void	SetReferenceTexture(CTexture *_pTexture){ pTexture = _pTexture; }

	// ゲッター
	SVector2D	GetPositionLeftTop(){ return positionLeftTop; }
	SVector2D	GetSizeLetter(){ return sizeLetter; }
	CTexture*	GetPTexture(){ return pTexture; }

	// 改行
	void	MoveToNewLine(){ AddPositionLeftTop(0, sizeLetter.y); }

private:
	SVector2D	positionLeftTop;	// 左上の座標(基準点となる)
	SVector2D	sizeLetter;			// 1文字の大きさ(表示の大きさ)
	SColor		color[VERTEX_MAX];	
	CTexture	*pTexture;

	void	RenderLetter(int _strIndex, char _char);		// 1文字を描画する関数

	void	SetVertexFormat();	// 頂点フォーマットの指定
	void	SetRenderState();	// レンダーステートの指定
	void	SetTexture();		// 描画前に実際にテクスチャをセットする

	SCoord2D	GetTextureNo(char _char);	// 描画する文字のテクスチャの場所を取得
	SCoord2D	GetTextureNoOffset();		// アニメーション用連番テクスチャの上下左右の場所を調整する
	SVector2D	GetTextureDivSizeUV();		// 分割テクスチャの1つあたりのUV座標のサイズを取得


public:
	CPolygonText2D();
	~CPolygonText2D();
};

#endif