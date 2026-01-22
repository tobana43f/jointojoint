//==========================================================
// 四角形の板ポリゴンを扱う抽象クラス
// 2D、3Dへ派生する
// メモ)
// 　頂点と配列の対応(TRIANGLE_FAN)
// 　 01 
// 　 32
// 注意)
// 　継承先ではこの順番で描画すること。


#ifndef _BASE_SQUARE_POLYGON_H
#define _BASE_SQUARE_POLYGON_H

//==========================================================
// インクルード
#include	"SquarePolygonData.h"

//==========================================================
// クラス定義
class CBaseSquarePolygon {
public:
	//==========================================================
	//　定数

	// 前情報を保持するバッファーの大きさ
	enum EBuffer {
		DATA_BUFFER_SIZE = 3,			// 〇フレーム前の情報まで保持する。(注意 ： 必ず２以上)
	};

	//==========================================================
	// 関数

	// 基本的な関数
	void Init();
	void Destroy();
	virtual void Update();				// 特別な理由がない限りオーバーライドはしない
	virtual void Render() PURE_FUNC;	// 継承先で自由に描画する

	// 値に加算する関数(移動させるときなどに便利)
	void	AddPositionCenter(float _x, float _y, float _z){ GetPCurData()->positionCenter.x += _x; GetPCurData()->positionCenter.y += _y; GetPCurData()->positionCenter.z += _z; }
	void	AddAngleDegree(float _degX, float _degY, float _degZ){ GetPCurData()->angleDegree.x += _degX; GetPCurData()->angleDegree.y += _degY; GetPCurData()->angleDegree.z += _degZ; }
	void	AddSize(float _w, float _h){ GetPCurData()->size.x += _w; GetPCurData()->size.y += _h; }
	void	AddColor(int _r, int _g, int _b, int _a = 0);
	void	AddColorToVertex(CSquarePolygonData::EVertexID _vxId, int _r, int _g, int _b, int _a = 0);

	// セッター系
	void	SetPositionCenter(float _x, float _y, float _z){ GetPCurData()->positionCenter.x = _x; GetPCurData()->positionCenter.y = _y; GetPCurData()->positionCenter.z = _z; }
	void	SetPositionLeftTop(float _x, float _y, float _z);
	void	SetAngleDegree(float _degX, float _degY, float _degZ){ GetPCurData()->angleDegree.x = _degX; GetPCurData()->angleDegree.y = _degY; GetPCurData()->angleDegree.z = _degZ; }
	void	SetSize(float _w, float _h){ GetPCurData()->size.x = _w; GetPCurData()->size.y = _h; }
	void	SetColor(int _r, int _g, int _b, int _a = 255);
	void	SetColorToVertex(CSquarePolygonData::EVertexID _vxId, int _r, int _g, int _b, int _a = 255);
	void	SetReferenceTexture(CTexture *_pTexture){ GetPCurData()->pTexture = _pTexture; }

	void	SetAnmNo(int _anmNo);
	void	SetIsReverseX(bool _state){ GetPCurData()->isReverseX = _state; }
	void	SetIsReverseY(bool _state){ GetPCurData()->isReverseY = _state; }

	void	SetBlendType(CSquarePolygonData::EBlendType _type){ GetPCurData()->blendType = _type; }

	// ゲッター系
	SVector3D	GetPositionCenter(){ return GetPCurData()->positionCenter; }
	SVector3D*	GetPPositionCenter(){ return &GetPCurData()->positionCenter; }
	SVector3D	GetAngleDegree(){ return GetPCurData()->angleDegree; }
	SVector2D	GetSize(){ return GetPCurData()->size; }
	CTexture*	GetPTexture(){ return GetPCurData()->pTexture; }

	SColor		GetColor(CSquarePolygonData::EVertexID _vertex){ return GetPCurData()->color[_vertex]; }

	bool		GetIsAnmEnd(){ return GetPCurData()->isAnmEnd; }

	// 1フレーム前のポリゴンの情報を取得する
	CSquarePolygonData GetPrevData(){ return bufData[1]; }

	// 前情報を保持しているバッファーの配列を取得する
	CSquarePolygonData* const GetDataBuffer(){ return bufData; }

	// 頂点の座標を求める
	// メモ)
	// 　求まる頂点座標は、3Dのワールド座標系の座標となるため、
	// 　2Dで使用する際は、スクリーン座標系に変換する必要があるため、仮想関数にいオーバーライドする。
	virtual SVector3D	CalcVertexPosition(int _vertex);

protected:
	//==========================================================
	// 変数

	// ポリゴンの前情報を格納するバッファー
	// 0 : 現在の
	// n : n フレーム前の情報
	CSquarePolygonData bufData[DATA_BUFFER_SIZE];

	//==========================================================
	// 関数

	// 現在のデータへのアクセスの統一化
	CSquarePolygonData* GetPCurData(){ return &bufData[0]; }

	// 情報を保持するバッファーの初期化
	void	InitBuffer();

	// 前情報を保持するバッファーの更新
	void	UpdateBuffer();

	// 連番テクスチャのアニメーションの更新
	void	UpdateAnimation();

	// 描画前の設定を行う関数
	virtual void	SetVertexFormat() PURE_FUNC;	// 頂点フォーマットの指定(継承先で自由なフォーマットを指定)
	virtual void	SetRenderState();				// レンダーステートの指定(継承先で自由なフォーマットを指定)
	void			SetTexture();					// 描画前に実際にテクスチャをセットする

	// 継承後SetRenderState()で呼び出せば楽に指定できる。
	// メモ) 
	// 　異なる方法で描画する場合は呼び出さなくてもOK
	void		SetRenderStateReplace();		// 通常の合成
	void		SetRenderStateAdd();			// 加算合成をするときのレンダーステート
	void		SetRenderStateSub();			// 減算合成

	// 描画前のポリゴンの準備の際に使用する関数
	SCoord2D	GetTextureNo();			// アニメーション用連番テクスチャの番号の取得
	SCoord2D	GetTextureNoOffset();	// アニメーション用連番テクスチャの上下左右の場所を調整する
	SVector2D	GetTextureDivSizeUV();	// 分割テクスチャの1つあたりのUV座標のサイズを取得

public:
	CBaseSquarePolygon();
	CBaseSquarePolygon(CSquarePolygonData _src);	// データをもとに生成。
	virtual ~CBaseSquarePolygon();
};

#endif
