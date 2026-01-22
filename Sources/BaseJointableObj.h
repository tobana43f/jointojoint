//==========================================================
// ステージオブジェクトのテスト
#ifndef _INC_BASE_JOINTABLE_OBJECT_H
#define _INC_BASE_JOINTABLE_OBJECT_H

//==========================================================
// インクルード
#include	"BlockObject.h"
#include	"Allocator.h"
#include	"CircleCollider.h"
#include	"AllCharacterManager.h"

#include	"ControllerDefinition.h"
#include	"GameState.h"

//==========================================================
// 名前空間
using namespace NControllerDefinition;

//==========================================================
// マクロ定義
#undef  PURE_FUNC
#define PURE_FUNC =0

//==========================================================
// クラス定義
class CBaseJointableObject {
public:
	//==========================================================
	// 定数
	static const float BLOCK_SIZE;

	//==========================================================
	// 関数

	virtual void Init(SVector3D _initPos, float _initAngle = 0);
	virtual void Destroy();
	virtual void Update();
	virtual void Sync();
	virtual void Render();

	// 初期位置
	void SetInitPosition(SVector3D _posCenter){ initPositionCenter = _posCenter; }
	SVector3D GetInitPosition(){ return initPositionCenter; }


	// セッター
	static void SetTexRefBlock(CTexture *_pTex){ pTexRefBlock = _pTex; }

	void SetIsConnected(bool _state){ isConnected = _state; }
	bool GetIsConnected(){ return isConnected; }

	// ゲッター
	int				GetBlockNum() { return blockNum; }
	CBlockObject*	GetPBlkockObject(int _idx){ if (_idx < blockNum){ return &block[_idx]; } return NULL; }
	CBlockObject*	GetBlockObjArray(){ return block; }

	CCircleCollider* GetPCollideCoreSpace(){ return &colCoreSpace; }

	// 参照する情報の設定
	static void		SetRefCharacter(CAllCharacterManager *_pRefCharacter){ pRefCharacter = _pRefCharacter; }

	// ゲームの状態を参照する(ゴール時に使用)
	static void		SetRefGameState(CGameState *_pRef){ pRefGameState = _pRef; }

	static void SetRefInput1(CInput* _refInput){ pRefInput1 = _refInput; }

protected:
	//==========================================================
	// 変数

	static CInput *pRefInput1;	// ここでも使えるようにしておく

	// リソース
	static CTexture	*pTexRefBlock;

	// 全体のオブジェクトの
	SVector3D		positionLeftTop;
	float			angleDegree;
	
	CCircleCollider	colCoreSpace;
	bool			isConnected;

	// 使用するブロックオブジェクトの数
	int				blockNum;

	// 実際のブロックの配列(確保する)
	CBlockObject	*block;

	//==========================================================
	// 関数

	// 情報を取得
	CAllCharacterManager* GetPCharacter(){ return pRefCharacter; }
	CGameState*	GetPGameState(){ return pRefGameState; }

	// キャラクターを振り子のように操作する関数。(たぶん共通で利用する)
	virtual void UpdateCharacter() PURE_FUNC;

	// コアを外す処理。
	// メモ)
	// 　外すボタンで、外す処理は含んでいないので、
	// 　継承後に外す処理を実装する場合は、入力処理を書くこと。
	void ProcSeperateCore();

	// つながっている場合にキャラクターのコアの位置を合わせる
	void SyncCore();

private:
	//==========================================================
	// 必要となるリソース

	static CAllCharacterManager *pRefCharacter;
	static CGameState			*pRefGameState;

	SVector3D	initPositionCenter;

public:
	CBaseJointableObject();
	virtual ~CBaseJointableObject();
};

#endif