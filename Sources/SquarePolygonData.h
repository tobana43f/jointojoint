//==========================================================
// 基本的な四角形ポリゴンを描画するのに必要なデータをまとめたクラス
// メモ)
// 　もともとは、ポリゴンを扱うクラスの抽象クラスとして定義していたが、
// 　前情報をクラス内で保持しておこうと思い、データと処理を分離した。
// 　(メンバに自分のクラスの実態を持てないため。)
//----------------------------------------------------------
// 注意)
// 　このクラスは、ポリゴンを扱う基本クラスへの継承以外は使用しない。
#ifndef _INC_SQUARE_POLYGON_DATA_H
#define _INC_SQUARE_POLYGON_DATA_H

//==========================================================
// インクルード
#include	"MathTypeDefinition.h"	// 両方に対応した型の定義を参照
#include	"Texture.h"

#define PURE_FUNC =0

//==========================================================
// ポリゴンの情報を管理するクラス
class CSquarePolygonData {
public:
	//==========================================================
	// 定数

	// 各頂点を指定する定数
	enum EVertexID{
		VERTEX_0,	// 左上
		VERTEX_1,	// 右上
		VERTEX_2,	// 右下
		VERTEX_3,	// 左下
		VERTEX_MAX
	};

	// 描画時の合成方法
	enum EBlendType{
		REPLACE,		// 上書き(通常の合成方法)
		ADD,			// 加算合成
		SUB,			// 減算合成
	};

	//==========================================================
	// 変数

	SVector3D	positionCenter;			// 中心座標
	SVector3D	angleDegree;			// 角度（単位：度）（Z軸方向の角度）
	SVector2D	size;					// 大きさ（w,h）(直径)

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

	// 情報をすべて初期化
	// この関数だけなので、Cppは用意しない
	void Init(){
		// ポリゴンの情報
		positionCenter = SVector3D(0, 0, 0);
		angleDegree = SVector3D(0, 0, 0);
		size = SVector2D(0, 0);
	
		for (int i = 0; i < VERTEX_MAX; ++i){
			color[i].r = 255;
			color[i].g = 255;
			color[i].b = 255;
			color[i].a = 255;
		}

		pTexture = NULL;

		// アニメーション関係
		anmCnt = 0;
		anmNo = 0;
		isReverseX = false;
		isReverseY = false;

		// ブレンドタイプ
		blendType = REPLACE;
	}

public:
	CSquarePolygonData(){};
	virtual ~CSquarePolygonData(){};
};

#endif