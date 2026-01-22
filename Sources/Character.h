//==========================================================
// プレイヤーが操作するキャラクタークラス
// メモ)
// 　ばねサンプル用に基底クラスとして定義しているが、
// 　実際に実装する際は、汎用のGameObjectクラスを作り、それを継承して作成すること。
//----------------------------------------------------------
// 詳細なメモ。(クラス設計に関して)
// プレイヤーはコアを使ってキャラクター以外のオブジェクトにも接続できるので、
// GameObjectから、接続できるオブジェクト、できないオブジェクトを別々のクラスで定義し、
// その派生クラスをさらに継承して接続できるかどうかを判断する。
//----------------------------------------------------------
// キャラクターの設計
// キャラクターはその他のオブジェクトにつなぐので、リストで管理する。
// 次につなげるオブジェクトのポインタをメンバに持つ。
//----------------------------------------------------------
// 前後のキャラクターの参照を行うため、
// 双方向リストで管理する。
//----------------------------------------------------------
// コアをつないでいるときは、繋いだ先のボディの座標がコアの位置となる
// (繋いだ先のボディが、コアを操作する。)
#ifndef _INC_CHARACTER_H
#define _INC_CHARACTER_H

//==========================================================
// インクルード

#include	"Polygon3D.h"
#include	"PolygonLine.h"
#include	"Texture.h"

#include	"RigidBody.h"
#include	"CharacterBody.h"

#include	"CircleCollider.h"	
#include	"AABBCollider.h"

//==========================================================
// クラス定義

class CCharacter {
public:
	//==========================================================
	// 定数

	static float	GetSpringStiffness();		// バネの強さ
	static float	GetCoreMass();				// コアの重さ
	static float	GetBodyMass();				// ボディの重さ
	static float	GetCableNaturalLength();	// ケーブルの自然長

	static float	GetCableWidth();			// ケーブルの幅、

	static int		GetCntCoreCollideInterval();				// 外した後の、判定無効時間

	enum EState {
		ACTIVE,			// 起動状態
		NEGATIVE,		// 非起動状態
		BROKEN,			// 落下後など
	};

	//==========================================================
	// 変数

	// 参照するリソースのポインタ
	// 注意)ここでは実態を持たない
	static CTexture	*pTexRefBodyActive;
	static CTexture	*pTexRefCoreActive;
	static CTexture *pTexRefCoreNegative;
	static CTexture	*pTexRefCable;

	//==========================================================
	// 関数
	void Init();
	void Update();
	void Sync();
	void Render();

	static void SetReferenceResource(CTexture *_pTexBody, CTexture *_pTexCore, CTexture *_pTexCoreNaga ,CTexture *_pTexCable);

	// 初期位置
	void SetInitPosition(SVector3D _posCenter){ initPositionCenter = _posCenter; }
	SVector3D GetInitPosition(){ return initPositionCenter; }

	// あたり判定を取得する
	CCircleCollider	GetColliderCoreSpace(){ return colliderCoreSpace; }
	CCircleCollider	GetColliderCore(){ return colliderCore; }
	CAABBCollider	GetColliderGround(){ return colGround; }

	void SetIsJumpReady(bool _state){ isJumpReady = _state; }
	bool GetIsJumpReady(){ return isJumpReady; }

	void SetIsConnected(bool _state){ isConnected = _state; }
	bool GetIsConnected(){ return isConnected; }
	//----------------------------------------------------------
	// ユーティリティ

	// コアのあたり判定の状態を設定する
	void ProcCoreSeperate(){ colliderCore.SetIsActive(false); cntCoreSeperate = cnt; }

	//==========================================================
	// 変数

	// ボディの剛体
	CCharacterBody	body;

	// サークル
	CCircleCollider	colliderCoreSpace;	// コアをつなげられる空間のあたり判定

	// コア
	CPolygon3D		polyCore;
	SVector3D		acceleartionCore;	// コアの加速度
	SVector3D		velocityCore;
	CCircleCollider	colliderCore;		// コアのあたり判定

	// ケーブル
	CPolygonLine	polyCable;

	// 着地常態かを判断するあたり判定
	CAABBCollider	colGround;
	SVector3D		initColGroundRelPos;	// 0度の時のあたり判定の位置

	CCharacter		*prev;	// 後ろにつながっているキャラクター	
	CCharacter		*next;	// 次につながっているキャラクター

private:
	//==========================================================
	// 変数

	// 汎用カウンタ
	int cnt;

	// コアを外した時のカウンタ
	int cntCoreSeperate;

	// ジャンプ可能フラグ
	bool isJumpReady;

	// コアをつなげているフラグ
	bool isConnected;

	// 初期位置
	// メモ)
	// 　ファイルからロードしたデータを入れて置き、Init関数でこの値に初期化する。
	SVector3D	initPositionCenter;

	//==========================================================
	// 関数

	// ケーブルの位置をボディとコアに同期させる処理
	void SyncCablePosition();	

	// バネのシミュレーション処理
	void ProcSpringMovement();	

	// コアの判定無効時間の判定
	void ProcCoreInterval();

	// 回転の補正
	void ProcAdjustRotate();

public:
	CCharacter();
	~CCharacter();

};

#endif