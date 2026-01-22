//==========================================================
// AABBでのあたり判定を行うクラス
#ifndef _INC_AABB_COLLIDER_H
#define _INC_AABB_COLLIDER_H

//==========================================================
// インクルード
#include	"BaseCollider.h"
#include	"MathTypeDefinition.h"

//==========================================================
// クラス定義
class CAABBCollider : public CBaseCollider {
public:
	//==========================================================
	// 定数

	enum EType{
		CONSTANT,	// 大きさや位置が変化しない
		WRAPPER,	// オブジェクトが回転した際に大きさを合わせる。
	};

	//==========================================================
	// 関数
	
	// オーナーになっているポリゴンをすべて包む形に更新する
	void Update();

	// タイプを設定
	void SetType(EType _type){ type = _type; }

	// サイズを設定
	void SetRelativeSize(float _x, float _y){ relativeSize = SVector3D(_x, _y, 0); }

	// サイズを取得
	SVector3D GetRelativeSize(){ return relativeSize; }

	// 実際の判定サイズを取得
	SVector3D GetSize(){ return size; }

private:
	//==========================================================
	// 変数

	// あたり判定のタイプ
	EType		type;
	
	// 実際のサイズ
	SVector3D	size;

	// オーナーのポリゴンのサイズからの変更分のサイズ
	SVector3D	relativeSize;

	//==========================================================
	// 関数

	void UpdateConstant();
	void UpdateWrapper();

	// 実際のサイズを設定
	void SetSize(SVector3D _size){ size = _size; }

public:
	CAABBCollider();
	~CAABBCollider();
};

#endif